#include <Core/Window.h>
#include <Core/Mesh.h>
#include <Core/Scene.h>
#include <Core/Renderer.h>

int main()
{
    int width = 1280;
    int height = 720;
    Window window(width, height);
    Renderer renderer(&window);
    Scene scene;
    scene.background = std::string("assets/environment/piazza_bologni_1k.hdr");
    const vec3 eye = vec3(0.0f, 0.0f, 4.0f);
    const vec3 target = vec3(0.0f, 0.0f, 0.0f);
    Camera camera(eye, target, vec3(0.0, 1.0, 0.0f), 45.0f, width / (float)height);

    Material material;
    material.albedoMap = "assets/models/helmet/Default_albedo.jpg";

    Mesh mesh;
    mesh.load("assets/models/helmet/DamagedHelmet.gltf");

    Model model(&mesh, &material);
    scene.model = model;

    // Transform transform(vec3(0.0f, 0.0f, -1.0f), quat(0.0, 0.0f, 0.0f, 1.0f), vec3(1.0f, 1.0f, 1.0f));

    while (!window.shouldClose())
    {
        window.poolEvents();
        auto euler = scene.model.transform.rotation();
        euler.y = glfwGetTime() * 0.5;

        Transform transform(vec3(0.0f, 0.0f, 0.0f), euler, vec3(1.0f, 1.0f, 1.0f));
        scene.model.transform = transform;
        
        renderer.render(scene, camera);

        window.swap();
    }
    window.destroy();
}
