#pragma once
#include <glad/glad.h>
#include <vector>
#include "../Common/Color.h"
#include "../Common/Point.h"

const int VERTEX_COMPONENTS_COUNT = 6;

struct Vertex
{
    float x, y;
    float r, g, b, a;
};

class Renderer
{
public:
    Renderer() = default;

    void DrawPrimitive(
        GLenum mode,
        const std::vector<float> &vertices,
        const float *transformMatrix,
        GLint transformUniformLocation,
        float thickness = 1.0f)
    {
        if (vertices.empty())
            return;

        if (transformUniformLocation != -1 && transformMatrix != nullptr)
        {
            glUniformMatrix4fv(transformUniformLocation, 1, GL_FALSE, transformMatrix);
        }

        GLuint VAO, VBO;
        SetupBuffers(VAO, VBO, vertices);

        if (mode == GL_LINES)
        {
            glLineWidth(thickness);
        }

        glDrawArrays(mode, 0, vertices.size() / VERTEX_COMPONENTS_COUNT);
        CleanupBuffers(VAO, VBO);
    }

    std::vector<float> CreateLineVertices(
        const std::vector<Point> &points,
        const Color &color,
        bool closed = false)
    {
        if (points.size() < 2)
            return {};

        std::vector<float> vertices;

        for (size_t i = 0; i < points.size() - 1; i++)
        {
            AddLineSegment(vertices, points[i], points[i + 1], color);
        }

        if (closed && points.size() > 2)
        {
            AddLineSegment(vertices, points.back(), points.front(), color);
        }

        return vertices;
    }

    std::vector<float> CreateTriangleVertices(
        const std::vector<Point> &points,
        const std::vector<unsigned int> &indices,
        const Color &color)
    {
        std::vector<float> vertices;
        for (unsigned int idx : indices)
        {
            const Point &p = points[idx];
            vertices.insert(vertices.end(), {p.x, p.y, color.r, color.g, color.b, color.a});
        }

        return vertices;
    }

private:
    void SetupBuffers(GLuint &VAO, GLuint &VBO, const std::vector<float> &vertices)
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STREAM_DRAW);

        // Position attribute
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        // Color attribute
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);
    }

    void CleanupBuffers(GLuint VAO, GLuint VBO)
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }

    void AddLineSegment(std::vector<float> &vertices, const Point &p0, const Point &p1, const Color &color)
    {
        vertices.insert(vertices.end(), {p0.x, p0.y, color.r, color.g, color.b, color.a,
                                         p1.x, p1.y, color.r, color.g, color.b, color.a});
    }
};