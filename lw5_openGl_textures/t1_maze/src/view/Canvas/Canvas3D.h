#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "./ICanvas3D.h"
#include "./Window.h"
#include "./Shader/ShaderManager.h"
#include "./Texture/TextureLoader.h"
#include "./Renderer.h"
#include <functional>
#include <cmath>

#include "./Shader/BasicShader.h"
#include "./Transform.h"

#include "./Controller/KeyboardController.h"
#include "./FpvCamera3D.h"
#include <iostream>

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

    void Run(UpdateCallback updateCallback) override
    {
        if (!Initialize())
            return;

        m_renderer.InitOIT(m_width, m_height);
        m_lastFrameTime = glfwGetTime();

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBlendEquation(GL_FUNC_ADD);

        while (!ShouldClose())
        {
            if (m_resizePending)
            {
                Resize(m_pendingWidth, m_pendingHeight);
                m_resizePending = false;
            }

            double currentTime = glfwGetTime();
            float deltaTime = static_cast<float>(currentTime - m_lastFrameTime);
            m_lastFrameTime = currentTime;
            m_keyboardController.Update(deltaTime);

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
        MeshTextured(mesh);

        m_shaderManager.Use();
        m_renderer.DrawMesh(mesh);
    }

    void MeshTextured(const MeshData &mesh) {
        m_textureCount = 0;
        if (mesh.textures.empty()) {
            m_textureLoader.unbindTextures();
        } else {
            m_textureCount = std::min((int)mesh.textures.size(), 2);
            for (int i = 0; i < m_textureCount; i++) {
                m_textureLoader.bindTexture(m_textureLoader.getTexture(mesh.textures[i]), i);
            }
        }
    }

    Transform &GetTransform()
    {
        return m_transform;
    }

    FpvCamera3D &GetCamera() {
        return m_camera;
    }

    KeyboardController &GetKeyboardController() {
        return m_keyboardController;
    }

private:
    TextureLoader m_textureLoader;
    int m_textureCount;
    ShaderManager m_shaderManager;
    KeyboardController m_keyboardController;
    FpvCamera3D m_camera;
    Renderer m_renderer;
    Transform m_transform;

    GLuint m_tempDepthTexture;
    GLuint m_tempTexture;
    GLuint m_tempFBO;

    double m_lastFrameTime;

    bool m_resizePending = false;
    unsigned m_pendingWidth = 0;
    unsigned m_pendingHeight = 0;

    bool Initialize()
    {
        if (!Window::Initialize())
            return false;

        m_keyboardController.Initialize();
        glfwSetWindowUserPointer(m_window, this);

        glfwSetFramebufferSizeCallback(m_window, FramebufferSizeCallbackStatic);
        glfwSetKeyCallback(m_window, KeyCallbackStatic);

        LoadShaders();
        LoadTextures();
        CreateFBO();
        return true;
    }

    void Resize(unsigned width, unsigned height)
    {
        if (width == 0 || height == 0)
        {
            return;
        }

        m_width = width;
        m_height = height;
        glViewport(0, 0, width, height);

        float aspect = (float)width / (float)height;
        m_camera.SetAspect(aspect);
        
        m_renderer.ResizeOIT(width, height);
        
        CleanupFBO();
        CreateFBO();
    }

    void LoadShaders()
    {
        m_shaderManager.AddShader<BasicShader>(
            "basic", 
            m_transform.GetTransform().GetGLMMatrix(), 
            m_camera.GetProjectionMatrix(), 
            m_camera.GetViewMatrix(), 
            m_camera.GetPosition(),
            m_textureCount
        );
        m_shaderManager.SetCurrent("basic");
    }

    void LoadTextures()
    {
        m_textureLoader.LoadTexture("./textures/brick.jpg");
        m_textureLoader.LoadTexture("./textures/wallpaper.jpg");
        m_textureLoader.LoadTexture("./textures/wood.jpg");
        m_textureLoader.LoadTexture("./textures/granit.jpg");
        m_textureLoader.LoadTexture("./textures/stone.jpg");
        m_textureLoader.LoadTexture("./textures/earth.jpg");
        m_textureLoader.LoadTexture("./textures/grass.jpg");
        m_textureLoader.LoadTexture("./textures/sky.jpg");
        m_textureLoader.LoadTexture("./textures/wallpaper2.jpg");
        m_textureLoader.LoadTexture("./textures/wall.jpg");
        m_textureLoader.LoadTexture("./textures/darkness.png");
        m_textureLoader.LoadTexture("./textures/darkness4.png");
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

    void CleanupFBO()
    {
        if (m_tempTexture) {
            glDeleteTextures(1, &m_tempTexture);
            m_tempTexture = 0;
        }
        if (m_tempDepthTexture) {
            glDeleteTextures(1, &m_tempDepthTexture);
            m_tempDepthTexture = 0;
        }
        if (m_tempFBO) {
            glDeleteFramebuffers(1, &m_tempFBO);
            m_tempFBO = 0;
        }
    }

    static void KeyCallbackStatic(GLFWwindow *window, int key, int scancode, int action, int mods)
    {
        auto *canvas = static_cast<Canvas3D *>(glfwGetWindowUserPointer(window));
        if (canvas)
        {
            canvas->m_keyboardController.HandleKeyEvent(key, scancode, action, mods);
        }
    }

    static void FramebufferSizeCallbackStatic(GLFWwindow *window, int width, int height)
    {   
        Canvas3D* canvas = static_cast<Canvas3D*>(glfwGetWindowUserPointer(window));
        if (canvas) {                
            canvas->m_resizePending = true;
            canvas->m_pendingWidth = width;
            canvas->m_pendingHeight = height;
        }
        
        glViewport(0, 0, width, height);
    }
};
