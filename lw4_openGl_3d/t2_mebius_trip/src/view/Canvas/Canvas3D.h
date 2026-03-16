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

#include "./Controller/KeyboardController.h"
#include "./Controller/MouseController.h"
#include "./OrbitCamera3D.h"

class Canvas3D : public ICanvas3D, protected Window
{
public:
    Canvas3D(unsigned width = 800, unsigned height = 600)
        : Window(width, height, "OpenGL Canvas")
    {
        float aspect = (float)width / (float)height;
        m_camera.SetPerspective(70, aspect);
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
    }

    void Run(UpdateCallback updateCallback) override
    {
        if (!Initialize())
            return;

        m_renderer.InitOIT(m_width, m_height);
        m_lastFrameTime = glfwGetTime();
        m_keyboardController.Initialize(m_window);

        while (!ShouldClose())
        {
            double currentTime = glfwGetTime();
            float deltaTime = static_cast<float>(currentTime - m_lastFrameTime);
            m_lastFrameTime = currentTime;
            m_keyboardController.Update(deltaTime);
            m_mouseController.ProcessEvents(m_window);

            glBindFramebuffer(GL_FRAMEBUFFER, m_tempFBO);

            glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            m_renderer.ClearOITBuffers();

            if (updateCallback)
            {
                updateCallback(*this, deltaTime);
            }

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            m_renderer.ResolveOIT(m_tempTexture, m_tempDepthTexture);

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
    KeyboardController m_keyboardController;
    MouseController m_mouseController;
    bool m_isRotation = false;

    OrbitCamera3D m_camera;
    Renderer m_renderer;
    Transform m_transform;

    double m_lastFrameTime;

    GLuint m_tempDepthTexture;
    GLuint m_tempTexture;
    GLuint m_tempFBO;

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
        SubscribeUpdateCamera();
        CreateFBO();

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBlendEquation(GL_FUNC_ADD);
        return true;
    }

    void LoadShaders()
    {
        m_shaderManager.AddShader<BasicShader>(
            "basic", 
            m_transform.GetTransform().GetGLMMatrix(), 
            m_camera.GetProjectionMatrix(), 
            m_camera.GetViewMatrix(),
            m_camera.GetPosition()
        );
        m_shaderManager.SetCurrent("basic");
    }

    void CreateFBO()
    {
        glGenTextures(1, &m_tempTexture);
        glBindTexture(GL_TEXTURE_2D, m_tempTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0,
                     GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glGenTextures(1, &m_tempDepthTexture);
        glBindTexture(GL_TEXTURE_2D, m_tempDepthTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, m_width, m_height, 0,
                     GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glGenFramebuffers(1, &m_tempFBO);
        glBindFramebuffer(GL_FRAMEBUFFER, m_tempFBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                               GL_TEXTURE_2D, m_tempTexture, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                               GL_TEXTURE_2D, m_tempDepthTexture, 0);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void SubscribeUpdateCamera()
    {
        m_keyboardController.OnKeyHoldSubscribe(
            this, GLFW_KEY_LEFT, [this](float deltatime)
            { m_camera.Rotate(-50.0f * deltatime, 0); });

        m_keyboardController.OnKeyHoldSubscribe(
            this, GLFW_KEY_RIGHT, [this](float deltatime)
            { m_camera.Rotate(50.0f * deltatime, 0); });

        m_keyboardController.OnKeyHoldSubscribe(
            this, GLFW_KEY_UP, [this](float deltatime)
            { m_camera.Rotate(0, -50.0f * deltatime); });

        m_keyboardController.OnKeyHoldSubscribe(
            this, GLFW_KEY_DOWN, [this](float deltatime)
            { m_camera.Rotate(0, 50.0f * deltatime); });

        m_keyboardController.OnKeyHoldSubscribe(
            this, GLFW_KEY_W,
            [this](float deltatime)
            {
                m_camera.Zoom(-5.0f * deltatime);
            });

        m_keyboardController.OnKeyHoldSubscribe(
            this, GLFW_KEY_S,
            [this](float deltatime)
            {
                m_camera.Zoom(5.0f * deltatime);
            });

        m_mouseController.OnMoveSubscribe([this](const Point &mousePos){
            if (m_isRotation) {
                auto deltaPos = (m_mouseController.GetPrevMousePos() - mousePos) * 0.5;
                m_camera.Rotate(deltaPos.x, deltaPos.y);
            }
        });

        m_mouseController.OnPressSubscribe([this](const Point &mousePos){
            m_isRotation = true;
        });

        m_mouseController.OnReleaseSubscribe([this](const Point &mousePos){
            m_isRotation = false;
        });
    }
};
