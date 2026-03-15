#pragma once
#include <glad/glad.h>
#include <memory>
#include "../../../common/TransformMatrix.h"

class DynamicBufferManager
{
public:
    struct DynamicBuffers
    {
        GLuint VAO = 0;
        GLuint VBO = 0;
        TransformMatrix transform;
        size_t vertexCount = 0;
        size_t maxVertices = 0;
        GLenum mode = GL_TRIANGLES;
        float thickness = 1.0f;

        bool IsValid() const { return VAO != 0 && VBO != 0; }
    };

    DynamicBufferManager(GLint transformUniformLocation, int vertexComponentCount)
        : m_transformUniformLocation(transformUniformLocation),
          m_vertexComponentCount(vertexComponentCount){};
    ~DynamicBufferManager()
    {
        Cleanup();
    }

    void BeginDraw()
    {
        m_gpuBuffers.vertexCount = 0;
    }

    void Draw(
        GLenum mode,
        const std::vector<float> &vertices,
        const TransformMatrix &transform,
        float thickness = 1.0f)
    {
        bool transformChanged = !(m_gpuBuffers.transform.GetGLMMatrix() == transform.GetGLMMatrix());
        bool modeChanged = m_gpuBuffers.mode != mode;
        bool thicknessChanged = m_gpuBuffers.thickness != thickness;

        if (transformChanged || modeChanged || thicknessChanged)
        {
            FlushBuffers();
            SetBuffersData(mode, transform, thickness);
        }

        AddVerticesToBuffers(vertices);
    }

    void EndDraw()
    {
        FlushBuffers();
    }

    void Cleanup()
    {
        if (m_gpuBuffers.VAO)
        {
            glDeleteVertexArrays(1, &m_gpuBuffers.VAO);
            m_gpuBuffers.VAO = 0;
        }
        if (m_gpuBuffers.VBO)
        {
            glDeleteBuffers(1, &m_gpuBuffers.VBO);
            m_gpuBuffers.VBO = 0;
        }
        m_gpuBuffers.maxVertices = 0;
        m_gpuBuffers.vertexCount = 0;
    }

    void FlushBuffers()
    {
        if (m_gpuBuffers.vertexCount == 0)
        {
            return;
        }

        glBindVertexArray(m_gpuBuffers.VAO);

        if (m_transformUniformLocation != -1)
        {
            glUniformMatrix4fv(m_transformUniformLocation, 1, GL_FALSE, m_gpuBuffers.transform.GetMatrix());
        }

        if (m_gpuBuffers.mode == GL_LINES)
        {
            glLineWidth(m_gpuBuffers.thickness);
        }

        glDrawArrays(m_gpuBuffers.mode, 0, m_gpuBuffers.vertexCount);

        m_gpuBuffers.vertexCount = 0;
    }

private:
    DynamicBuffers m_gpuBuffers;
    GLint m_transformUniformLocation;
    int m_vertexComponentCount;

    void SetBuffersData(GLenum mode, const TransformMatrix &transform, float thickness = 1.0f)
    {
        m_gpuBuffers.mode = mode;
        m_gpuBuffers.transform = transform;
        m_gpuBuffers.thickness = thickness;
    }

    void AddVerticesToBuffers(const std::vector<float> &vertices)
    {
        size_t vertexCount = vertices.size() / m_vertexComponentCount;
        EnsureCapacity(m_gpuBuffers.vertexCount + vertexCount);

        glBindBuffer(GL_ARRAY_BUFFER, m_gpuBuffers.VBO);
        glBufferSubData(GL_ARRAY_BUFFER,
                        m_gpuBuffers.vertexCount * m_vertexComponentCount * sizeof(float),
                        vertices.size() * sizeof(float),
                        vertices.data());

        m_gpuBuffers.vertexCount += vertexCount;
    }

    void EnsureCapacity(size_t requiredVertices)
    {
        if (!m_gpuBuffers.IsValid())
        {
            glGenVertexArrays(1, &m_gpuBuffers.VAO);
            glGenBuffers(1, &m_gpuBuffers.VBO);
            m_gpuBuffers.maxVertices = requiredVertices * 2;

            glBindVertexArray(m_gpuBuffers.VAO);
            glBindBuffer(GL_ARRAY_BUFFER, m_gpuBuffers.VBO);

            glBufferData(GL_ARRAY_BUFFER,
                         m_gpuBuffers.maxVertices * m_vertexComponentCount * sizeof(float),
                         nullptr,
                         GL_DYNAMIC_DRAW);

            SetupAttributes();
        }
        else if (requiredVertices > m_gpuBuffers.maxVertices)
        {
            FlushBuffers();
            m_gpuBuffers.maxVertices = requiredVertices * 2;

            glDeleteVertexArrays(1, &m_gpuBuffers.VAO);
            glDeleteBuffers(1, &m_gpuBuffers.VBO);

            glGenVertexArrays(1, &m_gpuBuffers.VAO);
            glGenBuffers(1, &m_gpuBuffers.VBO);

            glBindVertexArray(m_gpuBuffers.VAO);
            glBindBuffer(GL_ARRAY_BUFFER, m_gpuBuffers.VBO);
            glBufferData(GL_ARRAY_BUFFER,
                         m_gpuBuffers.maxVertices * m_vertexComponentCount * sizeof(float),
                         nullptr,
                         GL_DYNAMIC_DRAW);

            SetupAttributes();
        }
    }

    void SetupAttributes()
    {
        glBindVertexArray(m_gpuBuffers.VAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_gpuBuffers.VBO);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
                              m_vertexComponentCount * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE,
                              m_vertexComponentCount * sizeof(float), (void *)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);
    }
};