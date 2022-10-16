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
        glBindBufferRange(GL_UNIFORM_BUFFER, 0, cameraBuffer.handle, 0, CameraDataSize);
    }
    void update(const Camera &camera)
    {
        mat4 proj = camera.getProj();
        mat4 view = camera.getView();
        vec3 eye = camera.transform.position();

        // const vec3 eye = vec3(0.0f, 6.0f, 11.0f);
        // const vec3 target = vec3(0.0f, 4.0f, -1.0f);
        // auto view = glm::lookAt(eye, target, vec3(0.0, 1.0, 0.0f));
        // auto proj = perspective(glm::radians(45.0f), 1280.0f / 720.f, 0.01f, 1000.0f);

        CameraData cameraData = {.view = view, .proj = proj, .cameraPos = eye};

        glNamedBufferSubData(cameraBuffer.handle, 0, CameraDataSize, &cameraData);
    }
    GLBuffer cameraBuffer;
};
