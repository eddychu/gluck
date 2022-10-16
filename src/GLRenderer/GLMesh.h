#pragma once

#include <GLRenderer/GLProgram.h>

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