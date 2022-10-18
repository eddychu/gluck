#pragma once

#include <GLRenderer/GLProgram.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <Core/Transform.h>
using namespace glm;

const GLsizeiptr TransformDataSize = sizeof(mat4);

struct GLTransform
{
    GLTransform(int location)
        : transformBuffer(TransformDataSize, value_ptr(mat4(1.0f)), GL_DYNAMIC_STORAGE_BIT)
    {
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, location, transformBuffer.handle);
    }

    void update(const Transform &transform)
    {
        // const mat4 scale = glm::scale(mat4(1.0f), vec3(2.0f));
        // const mat4 rot = glm::rotate(mat4(1.0f), glm::radians(90.0f), vec3(1.0f, 0.0f, 0.0f));
        // const mat4 pos = glm::translate(mat4(1.0f), vec3(0.0f, 0.0f, -1.0f));
        // const mat4 m = glm::rotate(scale * rot * pos, (float)glfwGetTime() * 0.1f, vec3(0.0f, 0.0f, 1.0f));

        glNamedBufferSubData(transformBuffer.handle, 0, TransformDataSize, value_ptr(transform.localToWorld()));
    }

    GLBuffer transformBuffer;
};
