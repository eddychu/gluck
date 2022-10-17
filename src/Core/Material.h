#pragma once
#include <string>
#include <Common/uuid.h>
using namespace std;
struct Material
{
    Material() : id(uuid::generate_uuid_v4())
    {
    }
    string albedoMap;
    string emissiveMap;
    string normalMap;
    string metalRoughnessMap;
    string aoMap;
    string id;
};