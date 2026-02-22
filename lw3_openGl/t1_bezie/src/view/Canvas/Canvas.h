#pragma once
#include "../Common/Color.h"
#include "../Common/Point.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <functional>
#include "./MouseController.h"
#include "./ICanvas.h"

const int VERTEX_COMPONENTS_COUNT = 6; // 6 компонентов на вершину (x,y,r,g,b,a)

class Canvas : public ICanvas
{
public:
    using UpdateCallback = std::function<void(ICanvas &)>;

    Canvas(unsigned width = 800, unsigned height = 600)
        : m_width(width), m_height(height), m_color(0x0)
    {
    }

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

    bool GladInit()
    {
        if (!glfwInit())
        {
            std::cerr << "Failed to initialize GLFW" << std::endl;
            return false;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        m_window = glfwCreateWindow(m_width, m_height, "OpenGL Project", NULL, NULL);
        if (!m_window)
        {
            std::cerr << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return false;
        }

        glfwMakeContextCurrent(m_window);
        glfwSetFramebufferSizeCallback(
            m_window, [](GLFWwindow *win, int width, int height)
            { glViewport(0, 0, width, height); });

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cerr << "Failed to initialize GLAD" << std::endl;
            glfwTerminate();
            return false;
        }

        initShaders();

        glViewport(0, 0, m_width, m_height);
        return true;
    }

    void RunWindow(UpdateCallback updateCallback)
    {
        if (!GladInit())
        {
            return;
        }

        while (!glfwWindowShouldClose(m_window))
        {
            m_mouseController.ProcessEvents(m_window);

            glClearColor(0, 0, 0, 1);
            glClear(GL_COLOR_BUFFER_BIT);
            glUseProgram(m_shaderProgram);

            if (updateCallback)
            {
                updateCallback(*this);
            }

            glfwSwapBuffers(m_window);
            glfwPollEvents();
        }

        glDeleteProgram(m_shaderProgram);
        glfwTerminate();
    }

    void DrawLine(const Point &p0, const Point &p1, float thickness = 1.0f)
    {
        DrawPolygon({p0, p1}, thickness);
    }

    void DrawPolygon(const std::vector<Point> &points, float thickness = 1.0f, bool closed = false) override
    {
        if (points.size() < 2)
            return;

        float r, g, b, a;
        GetColorFloats(r, g, b, a);
        std::vector<float> vertices;

        for (size_t i = 0; i < points.size() - 1; i++)
        {
            Point np0 = NormalizePoint(points[i]);
            Point np1 = NormalizePoint(points[i + 1]);

            vertices.insert(vertices.end(), {np0.x, np0.y, r, g, b, a, np1.x, np1.y, r, g, b, a});
        }

        if (closed && points.size() > 2)
        {
            Point npLast = NormalizePoint(points.back());
            Point npFirst = NormalizePoint(points.front());

            vertices.insert(vertices.end(), {npLast.x, npLast.y, r, g, b, a, npFirst.x, npFirst.y, r, g, b, a});
        }

        DrawPrimitive(GL_LINES, vertices, thickness);
    }

    void FillPolygon(const std::vector<Point> &points)
    {
        if (points.size() < 3)
            return;

        float r, g, b, a;
        GetColorFloats(r, g, b, a);

        std::vector<float> vertices;
        Point np0 = NormalizePoint(points[0]);

        for (size_t i = 1; i < points.size() - 1; i++)
        {
            Point np1 = NormalizePoint(points[i]);
            Point np2 = NormalizePoint(points[i + 1]);

            vertices.insert(vertices.end(), {np0.x, np0.y, r, g, b, a,
                                             np1.x, np1.y, r, g, b, a,
                                             np2.x, np2.y, r, g, b, a});
        }

        DrawPrimitive(GL_TRIANGLES, vertices);
    }

    void DrawRect(const Point &position, const Size &size, float thickness = 1.0f)
    {
        DrawPolygon({{position.x, position.y},
                     {position.x + size.width, position.y},
                     {position.x + size.width, position.y + size.height},
                     {position.x, position.y + size.height}},
                    thickness, true);
    }

    void FillRect(const Point &position, const Size &size)
    {
        FillPolygon({{position.x, position.y},
                     {position.x + size.width, position.y},
                     {position.x + size.width, position.y + size.height},
                     {position.x, position.y + size.height}});
    }

    void DrawCubeBezie(const Point &p0, const Point &p1, const Point &p2, const Point &p3)
    {
        std::vector<Point> points;
        const float tStep = 0.001;

        for (float t = 0; t <= 1; t += tStep)
        {
            Point currPoint = GetBeziePoint(p0, p1, p2, p3, t);
            points.push_back(currPoint);
        }

        DrawPolygon(points);
    }

    MouseController &GetMouseController()
    {
        return m_mouseController;
    }

private:
    unsigned m_width;
    unsigned m_height;
    Color m_color;
    GLuint m_shaderProgram;
    MouseController m_mouseController;
    GLFWwindow *m_window;

    void initShaders()
    {
        const char *vertexShaderSource = R"(
            #version 460 core
            layout (location = 0) in vec2 aPos;
            layout (location = 1) in vec4 aColor;
            
            out vec4 vertexColor;
            
            void main() {
                gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
                vertexColor = aColor;
            }
        )";

        const char *fragmentShaderSource = R"(
            #version 460 core
            in vec4 vertexColor;
            out vec4 FragColor;
            
            void main() {
                FragColor = vertexColor;
            }
        )";

        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);

        int success;
        char infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            std::cerr << "Vertex shader compilation failed: " << infoLog << std::endl;
        }

        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);

        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
            std::cerr << "Fragment shader compilation failed: " << infoLog << std::endl;
        }

        m_shaderProgram = glCreateProgram();
        glAttachShader(m_shaderProgram, vertexShader);
        glAttachShader(m_shaderProgram, fragmentShader);
        glLinkProgram(m_shaderProgram);

        glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(m_shaderProgram, 512, NULL, infoLog);
            std::cerr << "Shader program linking failed: " << infoLog << std::endl;
        }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    Point GetBeziePoint(const Point &p0, const Point &p1, const Point &p2, const Point &p3, float t)
    {
        float oneMinusT = 1 - t;
        float squareOneMinusT = oneMinusT * oneMinusT;
        float cibeOneMinusT = squareOneMinusT * oneMinusT;
        float t2 = t * t;
        float t3 = t2 * t;

        float x =
            cibeOneMinusT * p0.x +
            3 * squareOneMinusT * t * p1.x +
            3 * oneMinusT * t2 * p2.x +
            t3 * p3.x;
        float y =
            cibeOneMinusT * p0.y +
            3 * squareOneMinusT * t * p1.y +
            3 * oneMinusT * t2 * p2.y +
            t3 * p3.y;

        return Point(x, y);
    }

    void SetupVAOAndVBO(GLuint &VAO, GLuint &VBO, const std::vector<float> &vertices)
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STREAM_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);
    }

    void CleanupVAOAndVBO(GLuint VAO, GLuint VBO)
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }

    void DrawPrimitive(GLenum mode, const std::vector<float> &vertices, float thickness = 1.0f)
    {
        if (vertices.empty())
            return;

        GLuint VAO, VBO;
        SetupVAOAndVBO(VAO, VBO, vertices);

        if (mode == GL_LINES)
        {
            glLineWidth(thickness);
        }

        glDrawArrays(mode, 0, vertices.size() / VERTEX_COMPONENTS_COUNT);
        CleanupVAOAndVBO(VAO, VBO);
    }

    void GetColorFloats(float &r, float &g, float &b, float &a) const
    {
        r = HexToFloat(m_color.r);
        g = HexToFloat(m_color.g);
        b = HexToFloat(m_color.b);
        a = HexToFloat(m_color.a);
    }

    float HexToFloat(u_int8_t hex) const
    {
        return hex / 255.0f;
    }

    float NormalizeX(float x) const
    {
        return (x / m_width) * 2.0f - 1.0f;
    }

    float NormalizeY(float y) const
    {
        return 1.0f - (y / m_height) * 2.0f;
    }

    Point NormalizePoint(const Point &p) const
    {
        return Point(NormalizeX(p.x), NormalizeY(p.y));
    }
};