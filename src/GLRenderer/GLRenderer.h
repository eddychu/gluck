#pragma once
#include <unordered_map>
#include <string>
using namespace std;
#include <GLRenderer/GLSkybox.h>
#include <GLRenderer/GLCamera.h>
#include <GLRenderer/GLMesh.h>
#include <GLRenderer/GLTransform.h>
#include <GLRenderer/GLMaterial.h>
#include <Core/Scene.h>
const int CAMERA_LOCATION = 0;
const int MODEL_TRANSFORM_LOCATION = 2;
class GLRenderer
{
public:
    GLRenderer(int width = 0, int height = 0)
        : width(width), height(0)
    {
    }

    void resize(int width, int height)
    {
        this->width = width;
        this->height = height;

        // glEnable(GL_DEPTH_TEST);
        // glDepthMask(GL_TRUE);
        // glEnable(GL_CULL_FACE);
        // glEnable(GL_MULTISAMPLE);
    }

    void updateMaterial(const string &name, const Material &material)
    {
        if (materials.find(name) == materials.end())
        {
            GLMaterial glMaterial(material);
            materials.insert(pair<string, GLMaterial>(name, std::move(glMaterial)));
        }
    }

    void updateCamera(const string &name, const Camera &camera)
    {
        if (cameras.find(name) == cameras.end())
        {
            GLCamera glCamera(CAMERA_LOCATION);
            cameras.insert(pair<string, GLCamera>(name, std::move(glCamera)));
        }
        cameras.at(name).update(camera);
    }

    void updateBackground(const string &name, const string &envmap)
    {
        if (backgrounds.find(name) == backgrounds.end())
        {
            GLSkybox background(envmap);
            backgrounds.insert(pair<string, GLSkybox>(name, std::move(background)));
        }
    }

    void updateMesh(const string &name, const Mesh &mesh)
    {
        if (meshes.find(name) == meshes.end())
        {
            const size_t kSizeIndices = sizeof(uint32_t) * mesh.indices.size();
            const size_t kSizeVertices = sizeof(VertexData) * mesh.vertices.size();
            GLMesh glMesh(mesh.indices.data(), (uint32_t)kSizeIndices, (float *)mesh.vertices.data(), (uint32_t)kSizeVertices);
            meshes.insert(pair<string, GLMesh>(name, std::move(glMesh)));
        }
    }

    void updateTransform(const string &name, const Transform &transform)
    {
        if (transforms.find(name) == transforms.end())
        {
            GLTransform glTransform(MODEL_TRANSFORM_LOCATION);
            transforms.insert(pair<string, GLTransform>(name, std::move(glTransform)));
        }
        transforms.at(name).update(transform);
    }

    void render(const Scene &scene, const Camera &camera)
    {
        updateCamera("default", camera);
        updateBackground("default", scene.background);

        updateMesh("default", *(scene.model.mesh));
        updateTransform("default", scene.model.transform);
        updateMaterial("default", *(scene.model.material));
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        backgrounds.at("default").draw();

        materials.at("default").use();

        meshes.at("default").draw();
    }

    unordered_map<string, GLSkybox> backgrounds;
    unordered_map<string, GLCamera> cameras;
    unordered_map<string, GLMesh> meshes;
    unordered_map<string, GLTransform> transforms;
    unordered_map<string, GLProgram> programs;
    unordered_map<string, GLMaterial> materials;
    int width;
    int height;
};
