#pragma once
#include <string>
#include <Core/Model.h>
#include <Common/uuid.h>
#include <Core/Light.h>
class Scene
{
public:
    Scene() : id(uuid::generate_uuid_v4())
    {
    }
    Model model;
    Light light;
    std::string background;
    std::string id;
};