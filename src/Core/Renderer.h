#pragma once
#include <Core/Scene.h>
#include <Core/Camera.h>
#include <GLRenderer/GLRenderer.h>
#include <Common/uuid.h>
class Renderer
{
public:
    Renderer(class Window *window)
        : m_window(window), m_renderer(window->width(), window->height()), id(uuid::generate_uuid_v4())
    {
    }

    void render(Scene &scene, Camera &camera)
    {
        auto newSize = m_window->querySize();
        m_renderer.resize(newSize[0], newSize[1]);
        m_renderer.render(scene, camera);
    }

private:
    Window *m_window;
    GLRenderer m_renderer;
    std::string id;
};