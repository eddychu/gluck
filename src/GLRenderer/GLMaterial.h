#pragma once
#include <GLRenderer/GLProgram.h>
#include <GLRenderer/GLTexture.h>
#include <Core/Material.h>

const int ALBEDO_MAP_LOCATION = 0;

struct GLMaterial
{
    GLMaterial(const Material &material)
        : albedoMap(GL_TEXTURE_2D, material.albedoMap.c_str())
    {
        glBindTextures(ALBEDO_MAP_LOCATION, 1, &albedoMap.handle);
    }

    void bindTextures()
    {
        glBindTextures(ALBEDO_MAP_LOCATION, 1, &albedoMap.handle);
    }

    void use()
    {
        // bindTextures();
        // glActiveTexture(GL_TEXTURE0 + ALBEDO_MAP_LOCATION);
        program.use();
    }

    GLTexture albedoMap;
    GLShader vertexShader = GLShader("assets/shaders/simple.vert", GL_VERTEX_SHADER);
    GLShader fragShader = GLShader("assets/shaders/simple.frag", GL_FRAGMENT_SHADER);
    GLProgram program = GLProgram{vertexShader, fragShader};
};