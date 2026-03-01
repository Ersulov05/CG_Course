#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../../common/Color.h"
#include "../../common/Point.h"
#include "./ICanvas.h"
#include "./MouseController.h"
#include "./Triangulate.h"
#include "./Transformable.h"
#include "./Window.h"
#include "./ShaderProgram.h"
#include "./Renderer.h"
#include <functional>
#include <cmath>
#include "./KeyboardController.h"

class Canvas : public ICanvas, public Transformable, protected Window
{
public:
    using UpdateCallback = std::function<void(ICanvas &, float deltaTime)>;

    Canvas(unsigned width = 800, unsigned height = 600)
        : Window(width, height, "OpenGL Canvas"), m_color(0x0)
    {
        UpdateProjectionMatrix();
    }

    void PushMatrix() override { Transformable::PushMatrix(); }
    void PopMatrix() override { Transformable::PopMatrix(); }
    void Translate(float x, float y) override { Transformable::Translate(x, y); }
    void Translate(const Point &p) override { Transformable::Translate(p); }
    void Rotate(float angleDegrees) override { Transformable::Rotate(angleDegrees); }
    void Scale(float scaleX, float scaleY) override { Transformable::Scale(scaleX, scaleY); }
    void Scale(float scale) override { Transformable::Scale(scale); }
    void Scale(const Point &scale) override { Transformable::Scale(scale); }
    void ResetTransform() override { Transformable::ResetTransform(); }

    unsigned GetWidth() const override
    {
        return m_width;
    }

    unsigned GetHeight() const override
    {
        return m_height;
    }

    void SetColor(Color color)
    {
        m_color = color;
    }

    void Resize(unsigned width, unsigned height)
    {
        m_width = width;
        m_height = height;
        glViewport(0, 0, width, height);
        UpdateProjectionMatrix();
    }

    void RunWindow(UpdateCallback updateCallback)
    {
        if (!Initialize())
            return;

        if (!m_shaderProgram.Load("./shaders/basic.vert", "./shaders/basic.frag"))
        {
            std::cerr << "Failed to load shader program" << std::endl;
            return;
        }

        m_transformUniform = m_shaderProgram.GetUniformLocation("uTransform");
        m_projectionUniform = m_shaderProgram.GetUniformLocation("uProjection");
        m_renderer = Renderer(m_transformUniform);
        m_lastFrameTime = glfwGetTime();
        m_keyboardController.Initialize(m_window);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        while (!ShouldClose())
        {
            double currentTime = glfwGetTime();
            float deltaTime = static_cast<float>(currentTime - m_lastFrameTime);
            m_lastFrameTime = currentTime;

            m_mouseController.ProcessEvents(m_window);
            m_keyboardController.Update(deltaTime);

            glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            m_shaderProgram.Use();

            if (m_projectionUniform != -1)
            {
                glUniformMatrix4fv(m_projectionUniform, 1, GL_FALSE, glm::value_ptr(m_projectionMatrix));
            }

            m_renderer.BeginDraw();

            if (updateCallback)
            {
                updateCallback(*this, deltaTime);
            }

            m_renderer.EndDraw();

            SwapBuffers();
            PollEvents();
        }
    }

    void DrawLine(const Point &p0, const Point &p1, float thickness = 1.0f) override
    {
        DrawPolygon({p0, p1}, thickness);
    }

    void DrawPolygon(const std::vector<Point> &points, float thickness = 1.0f, bool closed = false) override
    {
        auto vertices = m_renderer.CreateLineVertices(points, m_color, closed);

        m_renderer.DrawPrimitive(GL_LINES, vertices, GetTransform(), thickness);
    }

    void FillPolygon(const std::vector<Point> &points) override
    {
        if (points.size() < 3)
            return;

        size_t hash = HashPoints(points);

        auto it = m_triangulationCache.find(hash);
        if (it == m_triangulationCache.end())
        {
            auto indices = Triangulate::Process(points);
            it = m_triangulationCache.insert({hash, std::move(indices)}).first;
        }

        auto vertices = m_renderer.CreateTriangleVertices(points, it->second, m_color);
        m_renderer.DrawPrimitive(GL_TRIANGLES, vertices, GetTransform());
    }

    void DrawCircle(const Point &center, float radius, float thickness = 1.0f, int segments = 32) override
    {
        if (radius <= 0 || segments < 3)
            return;

        std::vector<Point> points;
        points.reserve(segments + 1);

        float step = 2 * M_PI / segments;
        for (int i = 0; i <= segments; i++)
        {
            float angle = i * step;
            points.emplace_back(center.x + radius * cos(angle),
                                center.y + radius * sin(angle));
        }

        DrawPolygon(points, thickness, true);
    }

    void FillCircle(const Point &center, float radius, int segments = 32) override
    {
        if (radius <= 0 || segments < 3)
            return;

        std::vector<Point> points;
        float step = 2 * M_PI / segments;

        for (int i = 0; i < segments; i++)
        {
            float angle = i * step;
            points.emplace_back(center.x + radius * cos(angle),
                                center.y + radius * sin(angle));
        }

        FillPolygon(points);
    }

    void DrawRect(const Point &position, const Size &size, float thickness = 1.0f) override
    {
        DrawPolygon({{position.x, position.y},
                     {position.x + size.width, position.y},
                     {position.x + size.width, position.y + size.height},
                     {position.x, position.y + size.height}},
                    thickness, true);
    }

    void FillRect(const Point &position, const Size &size) override
    {
        FillPolygon({{position.x, position.y},
                     {position.x + size.width, position.y},
                     {position.x + size.width, position.y + size.height},
                     {position.x, position.y + size.height}});
    }

    MouseController &GetMouseController()
    {
        return m_mouseController;
    }

    KeyboardController &GetKeyboardController()
    {
        return m_keyboardController;
    }

private:
    Color m_color;
    MouseController m_mouseController;
    KeyboardController m_keyboardController;
    ShaderProgram m_shaderProgram;
    Renderer m_renderer;
    glm::mat4 m_projectionMatrix;
    GLint m_transformUniform;
    GLint m_projectionUniform;
    double m_lastFrameTime;

    using TriangulationCache = std::vector<unsigned int>;
    std::unordered_map<size_t, TriangulationCache> m_triangulationCache;

    void UpdateProjectionMatrix()
    {
        m_projectionMatrix = glm::ortho(
            0.0f, static_cast<float>(m_width),  // left, right
            static_cast<float>(m_height), 0.0f, // bottom, top (перевернуто для верхнего левого угла)
            -1.0f, 1.0f                         // near, far
        );
    }

    bool Initialize()
    {
        if (!Window::Initialize())
            return false;

        glfwSetWindowUserPointer(m_window, this);
        glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow *window, int width, int height)
                                       {
            Canvas* canvas = static_cast<Canvas*>(glfwGetWindowUserPointer(window));
            if (canvas) {
                canvas->Resize(width, height);
            } });

        return true;
    }

    size_t HashPoints(const std::vector<Point> &points)
    {
        size_t hash = points.size();
        for (const auto &p : points)
        {
            hash = hash * 31 + std::hash<float>{}(p.x);
            hash = hash * 31 + std::hash<float>{}(p.y);
        }
        return hash;
    }
};