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
    GLSkybox(const std::string &envMapFile = "assets/environment/piazza_bologni_1k.hdr",
             const std::string &envIrradianceMapFile = "assets/environment/piazza_bologni_1k_irradiance.hdr",
             const std::string &brdfLutFile = "assets/brdfLUT.ktx")
        : envMap(GL_TEXTURE_CUBE_MAP, envMapFile.c_str()), envIrradianceMap(GL_TEXTURE_CUBE_MAP, envIrradianceMapFile.c_str()), brdfLut(GL_TEXTURE_2D, brdfLutFile.c_str(), GL_CLAMP_TO_EDGE)
    {
        std::vector<GLuint> textures = {
            envMap.handle, envIrradianceMap.handle};

        glCreateVertexArrays(1, &vao);
        glBindTextures(5, textures.size(), textures.data());

        glBindTextureUnit(7, brdfLut.handle);
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
    GLTexture envIrradianceMap;
    GLTexture brdfLut;
    GLShader cubeVertexShader = GLShader("assets/shaders/cube.vert", GL_VERTEX_SHADER);
    GLShader cubeFragmentShader = GLShader("assets/shaders/cube.frag", GL_FRAGMENT_SHADER);

    GLProgram program = GLProgram{cubeVertexShader, cubeFragmentShader};
    GLuint vao;
};
