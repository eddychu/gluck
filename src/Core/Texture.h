#pragma once
#include <Common/uuid.h>



class Texture
{
public:
    Texture(const std::string &image_path) : image(image_path), id(uuid::generate_uuid_v4())
    {
    }

    std::string image;
    std::string id;
};