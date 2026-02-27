#pragma once
#include <glad/glad.h>
#include <vector>
#include "../Common/Color.h"
#include "../Common/Point.h"
#include "./DynamicBufferManager.h"

const int VERTEX_COMPONENTS_COUNT = 6;

struct Vertex
{
    float x, y;
    float r, g, b, a;
};

class Renderer
{
public:
    Renderer(GLint transformUniformLocation = -1)
        : m_bufferManager(transformUniformLocation, VERTEX_COMPONENTS_COUNT){};

    void BeginDraw()
    {
        m_bufferManager.BeginDraw();
    }

    void EndDraw()
    {
        m_bufferManager.EndDraw();
    }

    void DrawPrimitive(
        GLenum mode,
        const std::vector<float> &vertices,
        const TransformMatrix &transform,
        float thickness = 1.0f)
    {
        if (vertices.empty())
            return;

        m_bufferManager.Draw(mode, vertices, transform, thickness);
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
    DynamicBufferManager m_bufferManager;

    void AddLineSegment(std::vector<float> &vertices, const Point &p0, const Point &p1, const Color &color)
    {
        vertices.insert(vertices.end(), {p0.x, p0.y, color.r, color.g, color.b, color.a,
                                         p1.x, p1.y, color.r, color.g, color.b, color.a});
    }
};