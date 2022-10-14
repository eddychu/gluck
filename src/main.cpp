#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <vector>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <fstream>
#include <sstream>

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/cimport.h>
#include <assimp/version.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

using namespace glm;
using namespace std;

int getNumMipMapLevels2D(int w, int h)
{
    int levels = 1;
    while ((w | h) >> levels)
        levels += 1;
    return levels;
}

uint8_t *genDefaultCheckerboardImage(int *width, int *height)
{
    const int w = 128;
    const int h = 128;

    uint8_t *imgData = (uint8_t *)malloc(w * h * 3); // stbi_load() uses malloc(), so this is safe

    assert(imgData && w > 0 && h > 0);
    assert(w == h);

    if (!imgData || w <= 0 || h <= 0)
        return nullptr;
    if (w != h)
        return nullptr;

    for (int i = 0; i < w * h; i++)
    {
        const int row = i / w;
        const int col = i % w;
        imgData[i * 3 + 0] = imgData[i * 3 + 1] = imgData[i * 3 + 2] = 0xFF * ((row + col) % 2);
    }

    if (width)
        *width = w;
    if (height)
        *height = h;

    return imgData;
}

struct PerFrameData
{
    mat4 view;
    mat4 proj;
    vec3 cameraPos;
};

struct GLBuffer
{
    GLBuffer()
    {
    }
    GLBuffer(GLsizeiptr size, const void *data, GLbitfield flags)
    {

        glCreateBuffers(1, &handle);
        glNamedBufferStorage(handle, size, data, flags);
    }

    void destroy()
    {
        glDeleteBuffers(1, &handle);
    }

    GLuint handle;
};

struct GLShader
{
    GLShader()
    {
    }
    GLShader(const char *fileName, GLenum type)
        : handle(glCreateShader(type))
    {
        ifstream t(fileName);
        stringstream buffer;
        if (t.is_open() && t.good())
        {
            buffer << t.rdbuf();
            const string text_string = buffer.str();
            const char *text = text_string.c_str();

            glShaderSource(handle, 1, &text, nullptr);
            glCompileShader(handle);
            char message[8192];
            GLsizei length = 0;
            glGetShaderInfoLog(handle, sizeof(message), &length, message);

            if (length)
            {
                printf("%s (File: %s)\n", message, fileName);
                printf("shader file content: \n %s", text);
                throw std::runtime_error("failed to compile shader");
            }
        }
    }

    void destroy()
    {
        glDeleteShader(handle);
    }

    GLuint handle;
};

struct GLProgram
{
    GLProgram()
    {
    }
    GLProgram(const GLShader &shader1, const GLShader &shader2)
    {
        handle = glCreateProgram();
        glAttachShader(handle, shader1.handle);
        glAttachShader(handle, shader2.handle);
        glLinkProgram(handle);
        char buffer[8192];
        GLsizei length = 0;
        glGetProgramInfoLog(handle, sizeof(buffer), &length, buffer);
        if (length)
        {
            printf("%s\n", buffer);
            throw new std::runtime_error("failed to create program");
        }
    }

    void use()
    {
        glUseProgram(handle);
    }

    void destroy()
    {
        glDeleteProgram(handle);
    }

    GLuint handle;
};

struct GLTexture
{
    GLTexture()
    {
    }
    GLTexture(GLenum type_, const char *fileName)
        : type(type_)
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glCreateTextures(type, 1, &handle);
        glTextureParameteri(handle, GL_TEXTURE_MAX_LEVEL, 0);
        glTextureParameteri(handle, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(handle, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTextureParameteri(handle, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(handle, GL_TEXTURE_WRAP_T, GL_REPEAT);

        const char *ext = strrchr(fileName, '.');

        switch (type)
        {
        case GL_TEXTURE_2D:
        {
            int w = 0;
            int h = 0;
            int numMipmaps = 0;
            uint8_t *img = stbi_load(fileName, &w, &h, nullptr, STBI_rgb_alpha);
            if (!img)
            {
                img = genDefaultCheckerboardImage(&w, &h);
                if (!img)
                {
                    throw std::runtime_error("failed to load image %s" + std::string(fileName));
                }
            }
            numMipmaps = getNumMipMapLevels2D(w, h);
            glTextureStorage2D(handle, numMipmaps, GL_RGBA8, w, h);
            glTextureSubImage2D(handle, 0, 0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, img);
            stbi_image_free((void *)img);
            glGenerateTextureMipmap(handle);
            glTextureParameteri(handle, GL_TEXTURE_MAX_LEVEL, numMipmaps - 1);
            glTextureParameteri(handle, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTextureParameteri(handle, GL_TEXTURE_MAX_ANISOTROPY, 16);
            break;
        }

        default:
        {
            throw std::runtime_error("not implemented!!!");
        }
        }
    }

    void destroy()
    {
        glDeleteTextures(1, &handle);
    }

    GLenum type;
    GLuint handle;
};

struct GLMesh
{
    GLMesh()
    {
    }
    GLMesh(const uint32_t *indices, uint32_t indicesSize, const float *vertexData, uint32_t verticesSize)
        : numIndices(indicesSize / sizeof(uint32_t)), indicesBuffer(indicesSize, indices, 0), verticesBuffer(verticesSize, vertexData, 0)
    {
        glCreateVertexArrays(1, &vao);
        glVertexArrayElementBuffer(vao, indicesBuffer.handle);
    }

    void draw()
    {
        glBindVertexArray(vao);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, verticesBuffer.handle);
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(numIndices), GL_UNSIGNED_INT, nullptr);
    }
    void destroy()
    {
        glDeleteVertexArrays(1, &vao);
    }
    GLuint vao;
    uint32_t numIndices;
    GLBuffer indicesBuffer;
    GLBuffer verticesBuffer;
};

struct VertexData
{
    vec3 pos;
    vec3 n;
    vec2 tc;
};

struct State
{
    GLMesh mesh;
    GLTexture albedo;
    GLBuffer modelMat;
    GLBuffer perFrameData;
    GLProgram program;
    int width;
    int height;
};

void run()
{
    State state;
    state.width = 1280;
    state.height = 720;
    if (!glfwInit())
    {
        throw std::runtime_error("glfwInit failed");
    }

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_SAMPLES, 4);
    auto window = glfwCreateWindow(state.width, state.height, "hello opengl", NULL, NULL);
    if (!window)
    {
        throw std::runtime_error("glfw create widnow failed!!");
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGL())
    {
        glfwDestroyWindow(window);
        throw std::runtime_error("gladloadGL failed.");
    }

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glEnable(GL_CULL_FACE);
    glEnable(GL_MULTISAMPLE);

    const GLsizeiptr kUniformBufferSize = sizeof(PerFrameData);
    GLBuffer perFrameDataBuffer(kUniformBufferSize, nullptr, GL_DYNAMIC_STORAGE_BIT);
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, perFrameDataBuffer.handle, 0, kUniformBufferSize);
    state.perFrameData = std::move(perFrameDataBuffer);

    GLShader vertex("assets/shaders/simple.vert", GL_VERTEX_SHADER);
    GLShader frag("assets/shaders/simple.frag", GL_FRAGMENT_SHADER);
    GLProgram program(vertex, frag);
    vertex.destroy();
    frag.destroy();
    state.program = std::move(program);

    const aiScene *scene = aiImportFile("assets/models/helmet/DamagedHelmet.gltf", aiProcess_Triangulate);
    if (!scene || !scene->HasMeshes())
    {
        throw runtime_error("failed to load scene file");
    }

    std::vector<VertexData> vertices;
    std::vector<uint32_t> indices;

    {
        const aiMesh *mesh = scene->mMeshes[0];
        for (unsigned i = 0; i != mesh->mNumVertices; i++)
        {
            const aiVector3D v = mesh->mVertices[i];
            const aiVector3D n = mesh->mNormals[i];
            const aiVector3D t = mesh->mTextureCoords[0][i];
            vertices.push_back({.pos = vec3(v.x, v.y, v.z), .n = vec3(n.x, n.y, n.z), .tc = vec2(t.x, 1.0f - t.y)});
        }
        for (unsigned i = 0; i != mesh->mNumFaces; i++)
        {
            for (unsigned j = 0; j != 3; j++)
            {
                indices.push_back(mesh->mFaces[i].mIndices[j]);
            }
        }
        aiReleaseImport(scene);
    }

    const size_t kSizeIndices = sizeof(uint32_t) * indices.size();
    const size_t kSizeVertices = sizeof(VertexData) * vertices.size();

    GLMesh mesh(indices.data(), (uint32_t)kSizeIndices, (float *)vertices.data(), (uint32_t)kSizeVertices);
    state.mesh = std::move(mesh);
    GLTexture texAlbedo(GL_TEXTURE_2D, "assets/models/helmet/Default_albedo.jpg");
    glBindTextures(0, 1, &texAlbedo.handle);
    state.albedo = std::move(texAlbedo);

    const mat4 m(1.0f);
    GLBuffer modelMatrices(sizeof(mat4), value_ptr(m), GL_DYNAMIC_STORAGE_BIT);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, modelMatrices.handle);
    state.modelMat = std::move(modelMatrices);
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        glfwGetFramebufferSize(window, &state.width, &state.height);
        const float ratio = state.width / (float)state.height;

        glViewport(0, 0, state.width, state.height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        const mat4 p = glm::perspective(45.0f, ratio, 0.5f, 5000.0f);
        const vec3 eye = vec3(0.0f, 6.0f, 11.0f);
        const vec3 target = vec3(0.0f, 4.0f, -1.0f);
        const mat4 view = glm::lookAt(eye, target, vec3(0.0, 1.0, 0.0));
        const PerFrameData perFrameData = {
            .view = view,
            .proj = p,
            .cameraPos = eye};
        glNamedBufferSubData(state.perFrameData.handle, 0, kUniformBufferSize, &perFrameData);

        const mat4 scale = glm::scale(mat4(1.0f), vec3(5.0f));
        const mat4 rot = glm::rotate(mat4(1.0f), glm::radians(90.0f), vec3(1.0f, 0.0f, 0.0f));
        const mat4 pos = glm::translate(mat4(1.0f), vec3(0.0f, 0.0f, -1.0f));
        const mat4 m = glm::rotate(scale * rot * pos, (float)glfwGetTime() * 0.1f, vec3(0.0f, 0.0f, 1.0f));
        glNamedBufferSubData(state.modelMat.handle, 0, sizeof(mat4), value_ptr(m));
        state.program.use();
        state.mesh.draw();

        glfwSwapBuffers(window);
    }
    state.albedo.destroy();
    state.perFrameData.destroy();
    state.modelMat.destroy();
    state.mesh.destroy();
    state.program.destroy();

    glfwDestroyWindow(window);
    glfwTerminate();
}

int main()
{
    run();
    return 0;
}
