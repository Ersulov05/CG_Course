#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <functional>

class Window
{
public:
    Window(unsigned width = 800, unsigned height = 600, const char *title = "OpenGL Window")
        : m_width(width), m_height(height), m_title(title), m_window(nullptr)
    {
    }

    virtual ~Window()
    {
        Cleanup();
    }

    bool Initialize()
    {
        if (!glfwInit())
        {
            std::cerr << "Failed to initialize GLFW" << std::endl;
            return false;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        m_window = glfwCreateWindow(m_width, m_height, m_title, NULL, NULL);
        if (!m_window)
        {
            std::cerr << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return false;
        }

        glfwMakeContextCurrent(m_window);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cerr << "Failed to initialize GLAD" << std::endl;
            glfwTerminate();
            return false;
        }

        glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow *win, int width, int height)
                                       { glViewport(0, 0, width, height); });

        glViewport(0, 0, m_width, m_height);
        return true;
    }

    bool ShouldClose() const
    {
        return glfwWindowShouldClose(m_window);
    }

    void SwapBuffers()
    {
        glfwSwapBuffers(m_window);
    }

    void PollEvents()
    {
        glfwPollEvents();
    }

    GLFWwindow *GetGLFWWindow() const
    {
        return m_window;
    }

    unsigned GetWidth() const { return m_width; }
    unsigned GetHeight() const { return m_height; }

protected:
    void Cleanup()
    {
        if (m_window)
        {
            glfwDestroyWindow(m_window);
            m_window = nullptr;
        }
        glfwTerminate();
    }

    unsigned m_width;
    unsigned m_height;
    const char *m_title;
    GLFWwindow *m_window;
};