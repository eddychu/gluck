#pragma once
#include <string>
#include <Core/Model.h>
#include <Common/uuid.h>
class Scene
{
public:
    Scene() : id(uuid::generate_uuid_v4())
    {
    }
    Model model;
    std::string background;
    std::string id;
};