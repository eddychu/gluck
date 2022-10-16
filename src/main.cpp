#include <Core/Window.h>
#include <Core/Mesh.h>
#include <Core/Scene.h>
#include <Core/Renderer.h>
// struct VertexData
// {
//     vec3 pos;
//     vec3 n;
//     vec2 tc;
// };

// struct State
// {
//     GLCamera camera;
//     GLMesh mesh;
//     GLTexture albedo;
//     GLBuffer modelMat;
//     GLBuffer perFrameData;
//     GLProgram program;
//     GLFWwindow *window;
//     int width;
//     int height;
// };

// struct GLSkybox
// {
//     GLSkybox(const char *envMapFile = "assets/environment/piazza_bologni_1k.hdr")
//         : envMap(GL_TEXTURE_CUBE_MAP, envMapFile)
//     {
//         glCreateVertexArrays(1, &vao);
//         glBindTextures(0, 1, &envMap.handle);
//     }

//     void destroy()
//     {
//         glDeleteVertexArrays(1, &vao);
//         envMap.destroy();
//         cubeVertexShader.destroy();
//         cubeFragmentShader.destroy();
//         program.destroy();
//     }

//     void draw(State &state)
//     {
//         int width, height;
//         glfwGetFramebufferSize(state.window, &state.width, &state.height);
//         const float ratio = state.width / (float)state.height;

//         glViewport(0, 0, state.width, state.height);
//         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//         const vec3 eye = vec3(0.0f, 6.0f, 11.0f);
//         const vec3 target = vec3(0.0f, 4.0f, -1.0f);
//         Camera camera(eye, target, vec3(0.0, 1.0, 0.0f), 45.0f, state.width / (float)state.height);
//         const mat4 p = glm::perspective(45.0f, ratio, 0.5f, 5000.0f);
//         state.camera.perspectiveMat = std::move(p);
//         const PerFrameData perFrameData = {
//             .view = camera.viewMat,
//             .proj = camera.perspectiveMat,
//             .cameraPos = eye};
//         const GLsizeiptr kUniformBufferSize = sizeof(PerFrameData);
//         glNamedBufferSubData(state.perFrameData.handle, 0, kUniformBufferSize, &perFrameData);

//         program.use();
//         glDepthMask(false);
//         glBindVertexArray(vao);
//         glDrawArrays(GL_TRIANGLES, 0, 36);
//         glDepthMask(true);
//     }

//     GLTexture envMap;
//     GLShader cubeVertexShader = GLShader("assets/shaders/cube.vert", GL_VERTEX_SHADER);
//     GLShader cubeFragmentShader = GLShader("assets/shaders/cube.frag", GL_FRAGMENT_SHADER);
//     GLProgram program = GLProgram{cubeVertexShader, cubeFragmentShader};
//     GLuint vao;
//     GLBuffer perFrameData;
// };

// struct Scene
// {
//     void destroy()
//     {
//         skybox.destroy();
//     }
//     GLSkybox skybox;
// };

// struct GLRenderer
// {
//     GLRenderer(int width, int height)
//     {
//         state.width = width;
//         state.height = height;
//         init();
//     }

//     void init()
//     {
//         if (!glfwInit())
//         {
//             throw std::runtime_error("glfwInit failed");
//         }

//         glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//         glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
//         glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
//         glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
//         glfwWindowHint(GLFW_SAMPLES, 4);
//         state.window = glfwCreateWindow(state.width, state.height, "hello opengl", NULL, NULL);
//         if (!state.window)
//         {
//             throw std::runtime_error("glfw create widnow failed!!");
//         }
//         glfwMakeContextCurrent(state.window);

//         if (!gladLoadGL())
//         {
//             glfwDestroyWindow(state.window);
//             throw std::runtime_error("gladloadGL failed.");
//         }

//         const GLsizeiptr kUniformBufferSize = sizeof(PerFrameData);
//         GLBuffer perFrameDataBuffer(kUniformBufferSize, nullptr, GL_DYNAMIC_STORAGE_BIT);
//         glBindBufferRange(GL_UNIFORM_BUFFER, 0, perFrameDataBuffer.handle, 0, kUniformBufferSize);
//         state.perFrameData = std::move(perFrameDataBuffer);
//     }

//     void render(Scene &scene, Camera &camera)
//     {
//         state.camera = camera;
//         scene.skybox.draw(state);
//     }

//     void destroy()
//     {
//         // if (state.albedo.handle > 0)
//         // {
//         //     state.albedo.destroy();
//         // }
//         state.perFrameData.destroy();
//         // state.modelMat.destroy();
//         // state.mesh.destroy();
//         state.program.destroy();
//         glfwDestroyWindow(state.window);
//         glfwTerminate();
//     }
//     State state;
// };

// void run()
// {
//     int width = 1280;
//     int height = 720;
//     GLRenderer renderer(1280, 720);
//     const vec3 eye = vec3(0.0f, 6.0f, 11.0f);
//     const vec3 target = vec3(0.0f, 4.0f, -1.0f);
//     Camera camera(eye, target, vec3(0.0, 1.0, 0.0f), 45.0f, width / (float)height);
//     Scene scene;
//     while (!glfwWindowShouldClose(renderer.state.window))
//     {
//         glfwPollEvents();
//         renderer.render(scene, camera);
//         glfwSwapBuffers(renderer.state.window);
//     }
//     renderer.destroy();
// State state;
// state.width = 1280;
// state.height = 720;
// if (!glfwInit())
// {
//     throw std::runtime_error("glfwInit failed");
// }

// glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
// glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
// glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
// glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
// glfwWindowHint(GLFW_SAMPLES, 4);
// auto window = glfwCreateWindow(state.width, state.height, "hello opengl", NULL, NULL);
// if (!window)
// {
//     throw std::runtime_error("glfw create widnow failed!!");
// }
// glfwMakeContextCurrent(window);

// if (!gladLoadGL())
// {
//     glfwDestroyWindow(window);
//     throw std::runtime_error("gladloadGL failed.");
// }
// state.window = std::move(window);

// const GLsizeiptr kUniformBufferSize = sizeof(PerFrameData);
// GLBuffer perFrameDataBuffer(kUniformBufferSize, nullptr, GL_DYNAMIC_STORAGE_BIT);
// glBindBufferRange(GL_UNIFORM_BUFFER, 0, perFrameDataBuffer.handle, 0, kUniformBufferSize);
// state.perFrameData = std::move(perFrameDataBuffer);

// GLSkybox skybox;

// glEnable(GL_DEPTH_TEST);
// glDepthMask(GL_TRUE);
// glEnable(GL_CULL_FACE);
// glEnable(GL_MULTISAMPLE);

// const GLsizeiptr kUniformBufferSize = sizeof(PerFrameData);
// GLBuffer perFrameDataBuffer(kUniformBufferSize, nullptr, GL_DYNAMIC_STORAGE_BIT);
// glBindBufferRange(GL_UNIFORM_BUFFER, 0, perFrameDataBuffer.handle, 0, kUniformBufferSize);
// state.perFrameData = std::move(perFrameDataBuffer);

// GLShader vertex("assets/shaders/simple.vert", GL_VERTEX_SHADER);
// GLShader frag("assets/shaders/simple.frag", GL_FRAGMENT_SHADER);
// GLProgram program(vertex, frag);
// vertex.destroy();
// frag.destroy();
// state.program = std::move(program);

// const aiScene *scene = aiImportFile("assets/models/helmet/DamagedHelmet.gltf", aiProcess_Triangulate);
// if (!scene || !scene->HasMeshes())
// {
//     throw runtime_error("failed to load scene file");
// }

// std::vector<VertexData> vertices;
// std::vector<uint32_t> indices;

// {
//     const aiMesh *mesh = scene->mMeshes[0];
//     for (unsigned i = 0; i != mesh->mNumVertices; i++)
//     {
//         const aiVector3D v = mesh->mVertices[i];
//         const aiVector3D n = mesh->mNormals[i];
//         const aiVector3D t = mesh->mTextureCoords[0][i];
//         vertices.push_back({.pos = vec3(v.x, v.y, v.z), .n = vec3(n.x, n.y, n.z), .tc = vec2(t.x, 1.0f - t.y)});
//     }
//     for (unsigned i = 0; i != mesh->mNumFaces; i++)
//     {
//         for (unsigned j = 0; j != 3; j++)
//         {
//             indices.push_back(mesh->mFaces[i].mIndices[j]);
//         }
//     }
//     aiReleaseImport(scene);
// }

// const size_t kSizeIndices = sizeof(uint32_t) * indices.size();
// const size_t kSizeVertices = sizeof(VertexData) * vertices.size();

// GLMesh mesh(indices.data(), (uint32_t)kSizeIndices, (float *)vertices.data(), (uint32_t)kSizeVertices);
// state.mesh = std::move(mesh);
// GLTexture texAlbedo(GL_TEXTURE_2D, "assets/models/helmet/Default_albedo.jpg");
// glBindTextures(0, 1, &texAlbedo.handle);
// state.albedo = std::move(texAlbedo);

// // GLTexture envMap(GL_TEXTURE_CUBE_MAP, "assets/environment/piazza_bologni_1k.hdr");
// // glBindTextures(5, 1, &envMap.handle);

// const mat4 m(1.0f);
// GLBuffer modelMatrices(sizeof(mat4), value_ptr(m), GL_DYNAMIC_STORAGE_BIT);
// glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, modelMatrices.handle);
// state.modelMat = std::move(modelMatrices);

// const vec3 eye = vec3(0.0f, 6.0f, 11.0f);
// const vec3 target = vec3(0.0f, 4.0f, -1.0f);
// Camera camera(eye, target, vec3(0.0, 1.0, 0.0f), 45.0f, state.width / (float)state.height);

// while (!glfwWindowShouldClose(window))
// {
//     glfwPollEvents();

//     // glfwGetFramebufferSize(window, &state.width, &state.height);
//     // const float ratio = state.width / (float)state.height;

//     // glViewport(0, 0, state.width, state.height);
//     // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//     // const mat4 p = glm::perspective(45.0f, ratio, 0.5f, 5000.0f);
//     // camera.perspectiveMat = std::move(p);
//     // const PerFrameData perFrameData = {
//     //     .view = camera.viewMat,
//     //     .proj = camera.perspectiveMat,
//     //     .cameraPos = eye};
//     // glNamedBufferSubData(state.perFrameData.handle, 0, kUniformBufferSize, &perFrameData);

//     // const mat4 scale = glm::scale(mat4(1.0f), vec3(5.0f));
//     // const mat4 rot = glm::rotate(mat4(1.0f), glm::radians(90.0f), vec3(1.0f, 0.0f, 0.0f));
//     // const mat4 pos = glm::translate(mat4(1.0f), vec3(0.0f, 0.0f, -1.0f));
//     // const mat4 m = glm::rotate(scale * rot * pos, (float)glfwGetTime() * 0.1f, vec3(0.0f, 0.0f, 1.0f));
//     // glNamedBufferSubData(state.modelMat.handle, 0, sizeof(mat4), value_ptr(m));

//     // state.program.use();
//     // state.mesh.draw();
//     skybox.draw(state);

//     glfwSwapBuffers(window);
// }
// skybox.destroy();
// // state.albedo.destroy();
// // state.perFrameData.destroy();
// // state.modelMat.destroy();
// // state.mesh.destroy();
// // state.program.destroy();

// glfwDestroyWindow(state.window);
// glfwTerminate();
// }

int main()
{
    int width = 1280;
    int height = 720;
    Window window(width, height);
    Renderer renderer(&window);
    Scene scene;
    scene.background = std::string("assets/environment/piazza_bologni_1k.hdr");
    const vec3 eye = vec3(0.0f, 0.0f, 4.0f);
    const vec3 target = vec3(0.0f, 0.0f, 0.0f);
    Camera camera(eye, target, vec3(0.0, 1.0, 0.0f), 45.0f, width / (float)height);

    Material material;
    material.albedoMap = "assets/models/helmet/Default_albedo.jpg";

    Mesh mesh;
    mesh.load("assets/models/helmet/DamagedHelmet.gltf");

    Model model(&mesh, &material);
    scene.model = model;

    // Transform transform(vec3(0.0f, 0.0f, -1.0f), quat(0.0, 0.0f, 0.0f, 1.0f), vec3(1.0f, 1.0f, 1.0f));

    while (!window.shouldClose())
    {
        window.poolEvents();
        auto euler = scene.model.transform.rotation();
        euler.y = glfwGetTime() * 0.5;

        Transform transform(vec3(0.0f, 0.0f, 0.0f), euler, vec3(1.0f, 1.0f, 1.0f));
        scene.model.transform = transform;

        renderer.render(scene, camera);

        window.swap();
    }
    window.destroy();
}
