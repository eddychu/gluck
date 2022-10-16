#pragma once
#include <vector>

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/cimport.h>
#include <assimp/version.h>
#include <stdexcept>

struct VertexData
{
    vec3 pos;
    vec3 n;
    vec2 tc;
};

class Mesh
{
public:
    void load(const char *fileName)
    {
        const aiScene *scene = aiImportFile("assets/models/helmet/DamagedHelmet.gltf", aiProcess_Triangulate);
        if (!scene || !scene->HasMeshes())
        {
            throw std::runtime_error("failed to load scene file");
        }

        {
            const aiMesh *mesh = scene->mMeshes[0];
            for (unsigned i = 0; i != mesh->mNumVertices; i++)
            {
                const aiVector3D v = mesh->mVertices[i];
                const aiVector3D n = mesh->mNormals[i];
                const aiVector3D t = mesh->mTextureCoords[0][i];
                vertices.push_back({.pos = vec3(v.x, v.y, v.z), .n = vec3(n.x, n.y, n.z), .tc = vec2(t.x, 1.0f - t.y)});
            }
            for (unsigned i = 0; i != mesh->mNumFaces; i++)
            {
                for (unsigned j = 0; j != 3; j++)
                {
                    indices.push_back(mesh->mFaces[i].mIndices[j]);
                }
            }
            aiReleaseImport(scene);
        }
    }

    std::vector<VertexData> vertices;
    std::vector<uint32_t> indices;
};