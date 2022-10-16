#pragma once
#include <Core/Mesh.h>
#include <Core/Material.h>
#include <Core/Transform.h>
class Model
{
public:
    Model()
    {
    }
    Model(Mesh *mesh_, Material *material_)
        : mesh(mesh_), material(material_)
    {
    }

    Mesh *mesh;
    Material *material;
    Transform transform;
};