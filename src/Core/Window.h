#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <glm/glm.hpp>
#include <functional>
using namespace glm;

class Window
{
public:
    static void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
    {
        auto parent_window = static_cast<Window *>(glfwGetWindowUserPointer(window));
        parent_window->onMouseButton(button, action, mods);
    }

    static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
    {
        auto parent_window = static_cast<Window *>(glfwGetWindowUserPointer(window));
        parent_window->onKey(key, scancode, action, mods);
    }

    static void mouse_move_callback(GLFWwindow *window, double x, double y)
    {
        auto parent_window = static_cast<Window *>(glfwGetWindowUserPointer(window));
        parent_window->onMouseMove(x, y);
    }

    Window(int width, int height)
        : m_width(width), m_height(height)
    {
        if (!glfwInit())
        {
            throw std::runtime_error("glfwInit failed");
        }

        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
        // glfwWindowHint(GLFW_SAMPLES, 4);
        window = glfwCreateWindow(width, height, "hello opengl", NULL, NULL);
        if (!window)
        {
            throw std::runtime_error("glfw create widnow failed!!");
        }
        glfwMakeContextCurrent(window);

        if (!gladLoadGL())
        {
            glfwDestroyWindow(window);
            throw std::runtime_error("gladloadGL failed.");
        }

        glfwSetWindowUserPointer(window, this);
        glfwSetMouseButtonCallback(window, mouse_button_callback);
        glfwSetKeyCallback(window, key_callback);
        glfwSetCursorPosCallback(window, mouse_move_callback);
    }

    bool shouldClose()
    {
        return glfwWindowShouldClose(window);
    }

    void poolEvents()
    {
        glfwPollEvents();
    }

    void swap()
    {
        glfwSwapBuffers(window);
    }

    void destroy()
    {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    // TODO: this function does two thing: update the window size and return the window size.
    //  this is not very ideal, as it can cause confusing. need better way to handle this.
    vec2 querySize()
    {
        glfwGetFramebufferSize(window, &m_width, &m_height);
        return vec2(m_width, m_height);
    }

    int width()
    {
        return m_width;
    }

    int height()
    {
        return m_height;
    }

    GLFWwindow *window;
    int m_width;
    int m_height;

    std::function<void(int, int, int)> onMouseButton;
    std::function<void(int, int, int, int)> onKey;
    std::function<void(double, double)> onMouseMove;
};