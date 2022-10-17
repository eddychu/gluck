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
#include <GLRenderer/GLLight.h>
const int CAMERA_LOCATION = 0;
const int LIGHT_LOCATION = 1;
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

        glEnable(GL_DEPTH_TEST);
        // glDepthMask(GL_TRUE);
        // glEnable(GL_CULL_FACE);
        // glEnable(GL_MULTISAMPLE);
    }

    void updateMaterial(const Material &material)
    {
        if (materials.find(material.id) == materials.end())
        {
            GLMaterial glMaterial(material);
            materials.insert(pair<string, GLMaterial>(material.id, std::move(glMaterial)));
        }
    }

    void updateCamera(const Camera &camera)
    {
        if (cameras.find(camera.id) == cameras.end())
        {
            GLCamera glCamera(CAMERA_LOCATION);
            cameras.insert(pair<string, GLCamera>(camera.id, std::move(glCamera)));
        }
        cameras.at(camera.id).update(camera);
    }

    void updateBackground(const string &name, const string &envmap)
    {
        if (backgrounds.find(name) == backgrounds.end())
        {
            GLSkybox background(envmap);
            backgrounds.insert(pair<string, GLSkybox>(name, std::move(background)));
        }
    }

    void updateMesh(const Mesh &mesh)
    {
        if (meshes.find(mesh.id) == meshes.end())
        {
            const size_t kSizeIndices = sizeof(uint32_t) * mesh.indices.size();
            const size_t kSizeVertices = sizeof(VertexData) * mesh.vertices.size();
            GLMesh glMesh(mesh.indices.data(), (uint32_t)kSizeIndices, (float *)mesh.vertices.data(), (uint32_t)kSizeVertices);
            meshes.insert(pair<string, GLMesh>(mesh.id, std::move(glMesh)));
        }
    }

    void updateTransform(const Transform &transform)
    {
        if (transforms.find(transform.id) == transforms.end())
        {
            GLTransform glTransform(MODEL_TRANSFORM_LOCATION);
            transforms.insert(pair<string, GLTransform>(transform.id, std::move(glTransform)));
        }
        transforms.at(transform.id).update(transform);
    }

    void updateLight(const Light &light)
    {
        if (lights.find(light.id) == lights.end())
        {
            GLLight glLight(LIGHT_LOCATION);
            lights.insert(pair<string, GLLight>(light.id, std::move(glLight)));
        }
        lights.at(light.id).update(light);
    }

    void render(const Scene &scene, const Camera &camera)
    {
        updateCamera(camera);
        updateBackground(scene.id, scene.background);
        updateLight(scene.light);
        updateMesh(*(scene.model.mesh));
        updateTransform(scene.model.transform);
        updateMaterial(*(scene.model.material));
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        backgrounds.at(scene.id).draw();
        materials.at(scene.model.material->id).use();
        meshes.at(scene.model.mesh->id).draw();
    }

    unordered_map<string, GLSkybox> backgrounds;
    unordered_map<string, GLCamera> cameras;
    unordered_map<string, GLMesh> meshes;
    unordered_map<string, GLTransform> transforms;
    unordered_map<string, GLProgram> programs;
    unordered_map<string, GLMaterial> materials;
    unordered_map<string, GLLight> lights;
    int width;
    int height;
};
