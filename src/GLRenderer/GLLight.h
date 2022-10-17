#pragma once
#include <Core/Light.h>
#include <glad/glad.h>
#include <GLRenderer/GLProgram.h>
struct LightData
{
    vec3 position;
    vec3 intensity;
};

const GLsizeiptr LightDataSize = sizeof(LightData);

struct GLLight
{
    GLLight(int location)
        : lightBuffer(LightDataSize, nullptr, GL_DYNAMIC_STORAGE_BIT)
    {
        glBindBufferRange(GL_UNIFORM_BUFFER, location, lightBuffer.handle, 0, LightDataSize);
    }

    void update(const Light &light)
    {
        auto data = LightData{
            .position = light.transform.position(), .intensity = light.intennsity};
        glNamedBufferSubData(lightBuffer.handle, 0, LightDataSize, &data);
    }
    GLBuffer lightBuffer;
};
