

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

    /*scene.environment = "assets/environment/immenstadter_horn_2k.hdr";
    scene.environmentIrradiance = "assets/environment/immenstadter_horn_2k_irradiance.hdr";*/
    scene.environment = std::string("assets/environment/piazza_bologni_1k.hdr");
    scene.environmentIrradiance = std::string("assets/environment/piazza_bologni_1k_irradiance.hdr");
    const vec3 eye = vec3(0.0f, 0.0f, 4.0f);
    const vec3 target = vec3(0.0f, 0.0f, 0.0f);
    Camera camera(eye, target, vec3(0.0, 1.0, 0.0f), 45.0f, width / (float)height);

    Material material;
    material.albedoMap = "assets/models/helmet/Default_albedo.jpg";
    material.emissiveMap = "assets/models/helmet/Default_emissive.jpg";
    material.normalMap = "assets/models/helmet/Default_normal.jpg";
    material.metalRoughnessMap = "assets/models/helmet/Default_metalRoughness.jpg";
    material.aoMap = "assets/models/helmet/Default_AO.jpg";
    Mesh mesh;
    mesh.load("assets/models/helmet/DamagedHelmet.gltf");

    Model model(&mesh, &material);
    scene.model = model;

    auto lightTransform = Transform(vec3(1.0f, 1.0f, 1.0f));
    scene.light.transform = lightTransform;
    scene.light.intensity = vec3(1.0, 1.0, 1.0);

    // Transform transform(vec3(0.0f, 0.0f, -1.0f), quat(0.0, 0.0f, 0.0f, 1.0f), vec3(1.0f, 1.0f, 1.0f));
    bool mouseHolding = false;
    double lastMouseX = 0.0;
    double lastMouseY = 0.0;
    float pitch = 0.0f;
    float yaw = 0.0f;
    window.onMouseButton = [&](int button, int action, int mod)
    {
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        {
            mouseHolding = true;
            glfwGetCursorPos(window.window, &lastMouseX, &lastMouseY);
        }
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
        {
            mouseHolding = false;
        }
    };

    window.onKey = [&](int key, int scancode, int action, int mods)
    {
        if (key == GLFW_KEY_W && action == GLFW_PRESS)
        {
            auto position = camera.transform.position();
            position.z -= 0.1;
            camera.transform.setPosition(position);
        }
        else if (key == GLFW_KEY_S && action == GLFW_PRESS)
        {
            auto position = camera.transform.position();
            position.z += 0.1;
            camera.transform.setPosition(position);
        }
    };

    window.onMouseMove = [&](double x, double y)
    {
        if (mouseHolding)
        {
            float dx = static_cast<float>(x - lastMouseX);
            float dy = static_cast<float>(y - lastMouseY);
            yaw += 0.001 * dx;
            pitch += 0.001 * dy;

            pitch = glm::clamp(pitch, -glm::pi<float>() + 0.1f, glm::pi<float>() - 0.1f);
            lastMouseX = x;
            lastMouseY = y;
        }
    };

    while (!window.shouldClose())
    {
        window.poolEvents();

        auto radius = length(camera.transform.position() - scene.model.transform.position());

        camera.transform.setRot(glm::vec3(pitch, yaw, 0.0));

         auto euler = scene.model.transform.rotation();
         euler.y = glfwGetTime() * 0.5;

         Transform transform(vec3(0.0f, 0.0f, 0.0f), euler, vec3(1.0f, 1.0f, 1.0f));
         scene.model.transform = transform;

        renderer.render(scene, camera);

        window.swap();
    }
    window.destroy();
}
