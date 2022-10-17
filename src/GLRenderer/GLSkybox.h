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
