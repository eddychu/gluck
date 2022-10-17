#pragma once
#include <glad/glad.h>

#include <GLRenderer/GLProgram.h>
#include <Core/Camera.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>
struct CameraData
{
    mat4 view;
    mat4 proj;
    vec3 cameraPos;
};

const GLsizeiptr CameraDataSize = sizeof(CameraData);

struct GLCamera
{
    GLCamera(int location)
        : cameraBuffer(CameraDataSize, nullptr, GL_DYNAMIC_STORAGE_BIT)
    {
        glBindBufferRange(GL_UNIFORM_BUFFER, location, cameraBuffer.handle, 0, CameraDataSize);
    }
    void update(const Camera &camera)
    {
        mat4 proj = camera.getProj();
        mat4 view = camera.getView();
        vec3 eye = camera.transform.position();

        CameraData cameraData = {.view = view, .proj = proj, .cameraPos = eye};

        glNamedBufferSubData(cameraBuffer.handle, 0, CameraDataSize, &cameraData);
    }
    GLBuffer cameraBuffer;
};
