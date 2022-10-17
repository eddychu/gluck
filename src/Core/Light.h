#pragma once
#include <Common/uuid.h>
#include <Core/Transform.h>
#include <glm/glm.hpp>

using namespace glm;
class Light
{
public:
    Light() : id(uuid::generate_uuid_v4()), intennsity(1.0, 1.0, 1.0), transform()
    {
    }

    std::string id;
    glm::vec3 intennsity;
    Transform transform;
};