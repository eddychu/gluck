#pragma once
#include <GLRenderer/GLProgram.h>
#include <GLRenderer/GLTexture.h>
#include <Core/Material.h>

const int ALBEDO_MAP_LOCATION = 0;
const int EMISSIVE_MAP_LOCATION = 1;
const int NORMAL_MAP_LOCATION = 2;
const int METAL_ROUGHNESS_LOCATION = 3;
const int AO_LOCATION = 4;

struct GLMaterial
{
    GLMaterial(const Material &material)
        : albedoMap(GL_TEXTURE_2D, material.albedoMap.c_str()), emissiveMap(GL_TEXTURE_2D, material.emissiveMap.c_str()), normalMap(GL_TEXTURE_2D, material.normalMap.c_str()), metalRoughnessMap(GL_TEXTURE_2D, material.metalRoughnessMap.c_str()), aoMap(GL_TEXTURE_2D, material.aoMap.c_str())
    {
        std::vector<GLuint> textures = {
            albedoMap.handle, emissiveMap.handle, normalMap.handle, metalRoughnessMap.handle, aoMap.handle};

        glBindTextures(ALBEDO_MAP_LOCATION, textures.size(), textures.data());
    }

    void use()
    {
        // bindTextures();
        // glActiveTexture(GL_TEXTURE0 + ALBEDO_MAP_LOCATION);
        program.use();
    }

    GLTexture albedoMap;
    GLTexture emissiveMap;
    GLTexture normalMap;
    GLTexture metalRoughnessMap;
    GLTexture aoMap;
    GLShader vertexShader = GLShader("assets/shaders/simple.vert", GL_VERTEX_SHADER);
    GLShader fragShader = GLShader("assets/shaders/simple.frag", GL_FRAGMENT_SHADER);
    GLProgram program = GLProgram{vertexShader, fragShader};
};