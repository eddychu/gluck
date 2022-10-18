#pragma once
#include <Core/Light.h>
#include <glad/glad.h>
#include <GLRenderer/GLProgram.h>
struct LightData
{
    vec4 position;
    vec4 intensity;
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

        vec4 postion = vec4(light.transform.position(), 1.0);
        vec4 intenstiy = vec4(light.intennsity, 1.0);
        LightData data = {.position = postion, .intensity = intenstiy};
        printf("%f %f %f\n", data.intensity.x, data.intensity.y, data.intensity.z);
        glNamedBufferSubData(lightBuffer.handle, 0, LightDataSize, &data);
    }
    GLBuffer lightBuffer;
    Light defaultLight;
};
