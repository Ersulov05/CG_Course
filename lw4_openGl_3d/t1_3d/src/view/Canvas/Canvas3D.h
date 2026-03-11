#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "./ICanvas3D.h"
#include "./Window.h"
#include "./Shader/ShaderManager.h"
#include "./Renderer.h"
#include <functional>
#include <cmath>

#include "./Shader/BasicShader.h"
#include "./Transform.h"

class Canvas3D : public ICanvas3D, protected Window
{
public:
    Canvas3D(unsigned width = 800, unsigned height = 600)
        : Window(width, height, "OpenGL Canvas")
    {
        m_view = glm::lookAt(
            glm::vec3(0, 0, 0), // позиция камеры
            glm::vec3(0, 0, 1), // точка, куда смотрим
            glm::vec3(0, 1, 0)  // направление "вверх"
        );

        m_projection = glm::mat4(1.0f);
        UpdateProjectionMatrix();
    }

    unsigned GetWidth() const override
    {
        return m_width;
    }

    unsigned GetHeight() const override
    {
        return m_height;
    }

    void Resize(unsigned width, unsigned height)
    {
        m_width = width;
        m_height = height;
        glViewport(0, 0, width, height);
        UpdateProjectionMatrix();
    }

    void Run(UpdateCallback updateCallback) override
    {
        if (!Initialize())
            return;

        m_lastFrameTime = glfwGetTime();

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);

        // glBlendFunc(GL_SRC_ALPHA, GL_ONE);

        // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA,
        //                     GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        // glBlendEquation(GL_FUNC_ADD);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBlendEquation(GL_FUNC_ADD);

        while (!ShouldClose())
        {
            double currentTime = glfwGetTime();
            float deltaTime = static_cast<float>(currentTime - m_lastFrameTime);
            m_lastFrameTime = currentTime;

            glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            if (updateCallback)
            {
                updateCallback(*this, deltaTime);
            }

            SwapBuffers();
            PollEvents();
        }
    }

    void DrawMesh(const MeshData &mesh)
    {
        m_shaderManager.Use();
        m_renderer.DrawMesh(mesh);
    }

    Transform &GetTransform()
    {
        return m_transform;
    }

private:
    ShaderManager m_shaderManager;
    Renderer m_renderer;
    Transform m_transform;
    glm::mat4 m_projection;
    glm::mat4 m_view;

    double m_lastFrameTime;

    void UpdateProjectionMatrix()
    {
        // m_projection = glm::ortho(
        //     -2.0f, 2.0f, // left, right
        //     -2.0f, 2.0f, // bottom, top
        //     -1.0f, 1.0f  // near, far
        // );

        // std::cout << -(float)m_width / 2.0f << std::endl;

        m_projection = glm::ortho(
            -(float)m_width / 2, (float)m_width / 2,
            -(float)m_height / 2, (float)m_height / 2,
            -1000.0f, 1000.0f // near, far
        );

        // float aspect = static_cast<float>(m_width) / static_cast<float>(m_height);
        // m_projection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);

        float aspect = static_cast<float>(m_width) / static_cast<float>(m_height);
        m_projection = glm::perspective(
            glm::radians(45.0f), // угол обзора
            aspect,              // соотношение сторон
            0.1f,                // near plane
            1000.0f              // far plane
        );
    }

    bool Initialize()
    {
        if (!Window::Initialize())
            return false;

        glfwSetWindowUserPointer(m_window, this);
        glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow *window, int width, int height)
                                       {
            Canvas3D* canvas = static_cast<Canvas3D*>(glfwGetWindowUserPointer(window));
            if (canvas) {
                canvas->Resize(width, height);
            } });

        LoadShaders();
        return true;
    }

    void LoadShaders()
    {
        m_shaderManager.AddShader<BasicShader>("basic", m_transform.GetTransform().GetGLMMatrix(), m_projection, m_view);
        m_shaderManager.SetCurrent("basic");
    }
};