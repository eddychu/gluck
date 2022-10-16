#pragma once

#include <Core/Transform.h>

class Camera
{
public:
    Camera(vec3 eye, vec3 target, vec3 up, float fov, float aspect)
    {
        auto view = lookAt(eye, target, up);
        auto iview = inverse(view);
        transform = Transform::fromWorldMat(iview);
        projection = perspective(glm::radians(fov), aspect, 1.0f, 1000.0f);
    }

    mat4 getView() const
    {
        return transform.worldToLocal();
    }

    mat4 getProj() const
    {
        return projection;
    }

    Transform transform;
    mat4 projection;
};