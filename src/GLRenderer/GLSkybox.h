#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <GLRenderer/GLTexture.h>
#include <GLRenderer/GLProgram.h>

#include <string>

using namespace glm;

struct GLSkybox
{
    GLSkybox(const std::string &envMapFile = "assets/environment/piazza_bologni_1k.hdr")
        : envMap(GL_TEXTURE_CUBE_MAP, envMapFile.c_str())
    {
        glCreateVertexArrays(1, &vao);
        glBindTextures(5, 1, &envMap.handle);
    }

    void destroy()
    {
        glDeleteVertexArrays(1, &vao);
        envMap.destroy();
        cubeVertexShader.destroy();
        cubeFragmentShader.destroy();
        program.destroy();
    }

    void draw()
    {
        // int width, height;
        // glfwGetFramebufferSize(state.window, &state.width, &state.height);
        // const float ratio = state.width / (float)state.height;

        // glViewport(0, 0, state.width, state.height);
        // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // const vec3 eye = vec3(0.0f, 6.0f, 11.0f);
        // const vec3 target = vec3(0.0f, 4.0f, -1.0f);
        // Camera camera(eye, target, vec3(0.0, 1.0, 0.0f), 45.0f, state.width / (float)state.height);
        // const mat4 p = glm::perspective(45.0f, ratio, 0.5f, 5000.0f);
        // state.camera.perspectiveMat = std::move(p);
        // const PerFrameData perFrameData = {
        //     .view = camera.viewMat,
        //     .proj = camera.perspectiveMat,
        //     .cameraPos = eye};
        // const GLsizeiptr kUniformBufferSize = sizeof(PerFrameData);
        // glNamedBufferSubData(state.perFrameData.handle, 0, kUniformBufferSize, &perFrameData);

        program.use();
        glDepthMask(false);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glDepthMask(true);
    }

    GLTexture envMap;
    GLShader cubeVertexShader = GLShader("assets/shaders/cube.vert", GL_VERTEX_SHADER);
    GLShader cubeFragmentShader = GLShader("assets/shaders/cube.frag", GL_FRAGMENT_SHADER);
    GLProgram program = GLProgram{cubeVertexShader, cubeFragmentShader};
    GLuint vao;
};
