#pragma once
#include <Core/Mesh.h>
#include <Core/Material.h>
#include <Core/Transform.h>
#include <Common/uuid.h>
class Model
{
public:
    Model() : id(uuid::generate_uuid_v4())
    {
    }
    Model(Mesh *mesh_, Material *material_)
        : mesh(mesh_), material(material_)
    {
    }

    Mesh *mesh;
    Material *material;
    Transform transform;
    std::string id;
};