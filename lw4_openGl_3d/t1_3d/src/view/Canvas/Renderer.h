#pragma once
#include <glad/glad.h>
#include <vector>
#include <unordered_map>
#include "./RendererData.h"

class Renderer
{
public:
    Renderer(){};

    void DrawMesh(const MeshData &mesh)
    {
        if (!mesh.vertices.empty() && !mesh.indices.empty() && mesh.indices.size() % 3 == 0)
        {
            DrawTriangles(mesh.vertices, mesh.indices);
        }
        if (!mesh.edgeVertices.empty() && !mesh.edgeIndices.empty() && mesh.edgeIndices.size() % 2 == 0)
        {
            DrawLines(mesh.edgeVertices, mesh.edgeIndices);
        }
    }

    void ClearAllBuffers()
    {
        for (auto &pair : m_triangleBuffers)
        {
            DeleteBuffer(pair.second);
        }
        m_triangleBuffers.clear();

        for (auto &pair : m_lineBuffers)
        {
            DeleteBuffer(pair.second);
        }
        m_lineBuffers.clear();
    }

private:
    struct GPUBuffer
    {
        GLuint VAO = 0;
        GLuint VBO = 0;
        GLuint EBO = 0;
        size_t indexCount = 0;
    };

    struct GLVertex
    {
        float px, py, pz; // позиция
        float nx, ny, nz; // нормаль
        float r, g, b, a; // цвет
    };

    std::unordered_map<size_t, GPUBuffer>
        m_triangleBuffers;
    std::unordered_map<size_t, GPUBuffer> m_lineBuffers;

    void DrawTriangles(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices)
    {
        if (vertices.empty() || indices.empty())
            return;

        GPUBuffer &buffer = GetOrCreateBuffer(vertices, indices, m_triangleBuffers);

        glBindVertexArray(buffer.VAO);
        glDrawElements(GL_TRIANGLES, buffer.indexCount, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    void DrawLines(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices)
    {
        if (vertices.empty() || indices.empty())
            return;

        GPUBuffer &buffer = GetOrCreateBuffer(vertices, indices, m_lineBuffers);

        glLineWidth(3.0f);
        glBindVertexArray(buffer.VAO);
        glDrawElements(GL_LINES, buffer.indexCount, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    GPUBuffer &GetOrCreateBuffer(
        const std::vector<Vertex> &vertices,
        const std::vector<unsigned int> &indices,
        std::unordered_map<size_t, GPUBuffer> &bufferCache)
    {
        size_t hash = HashVertexData(vertices, indices);

        auto it = bufferCache.find(hash);
        if (it != bufferCache.end())
        {
            return it->second;
        }

        GPUBuffer buffer;
        CreateBuffer(vertices, indices, buffer);

        bufferCache[hash] = buffer;
        return bufferCache[hash];
    }

    size_t HashVertexData(const std::vector<Vertex> &vertices,
                          const std::vector<unsigned int> &indices)
    {
        size_t hash = 0;

        auto combine = [](size_t &seed, size_t value)
        {
            seed ^= value + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        };

        combine(hash, vertices.size());
        combine(hash, indices.size());

        size_t vertexCount = std::min(vertices.size(), size_t(10));
        for (size_t i = 0; i < vertexCount; ++i)
        {
            combine(hash, std::hash<Vertex>()(vertices[i]));
        }

        size_t indexCount = std::min(indices.size(), size_t(10));
        for (size_t i = 0; i < indexCount; ++i)
        {
            combine(hash, std::hash<unsigned int>()(indices[i]));
        }

        return hash;
    }

    void CreateBuffer(const std::vector<Vertex> &vertices,
                      const std::vector<unsigned int> &indices,
                      GPUBuffer &buffer)
    {
        std::vector<GLVertex> glVertices;
        glVertices.reserve(vertices.size());

        for (const auto &v : vertices)
        {
            GLVertex glv;
            glv.px = v.position.x;
            glv.py = v.position.y;
            glv.pz = v.position.z;
            glv.nx = v.normal.x;
            glv.ny = v.normal.y;
            glv.nz = v.normal.z;
            glv.r = v.color.r;
            glv.g = v.color.g;
            glv.b = v.color.b;
            glv.a = v.color.a;
            glVertices.push_back(glv);
        }

        // Создаём VAO
        glGenVertexArrays(1, &buffer.VAO);
        glBindVertexArray(buffer.VAO);

        // Создаём VBO
        glGenBuffers(1, &buffer.VBO);
        glBindBuffer(GL_ARRAY_BUFFER, buffer.VBO);
        glBufferData(GL_ARRAY_BUFFER,
                     glVertices.size() * sizeof(GLVertex),
                     glVertices.data(),
                     GL_STATIC_DRAW);

        // Создаём EBO
        glGenBuffers(1, &buffer.EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     indices.size() * sizeof(unsigned int),
                     indices.data(),
                     GL_STATIC_DRAW);

        // Настройка атрибутов
        // Позиция (location = 0)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                              sizeof(GLVertex), (void *)0);
        glEnableVertexAttribArray(0);

        // Нормаль (location = 1)
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                              sizeof(GLVertex), (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // Цвет (location = 2)
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE,
                              sizeof(GLVertex), (void *)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);

        buffer.indexCount = indices.size();
    }

    void DeleteBuffer(GPUBuffer &buffer)
    {
        if (buffer.VAO)
            glDeleteVertexArrays(1, &buffer.VAO);
        if (buffer.VBO)
            glDeleteBuffers(1, &buffer.VBO);
        if (buffer.EBO)
            glDeleteBuffers(1, &buffer.EBO);
        buffer.VAO = buffer.VBO = buffer.EBO = 0;
        buffer.indexCount = 0;
    }
};