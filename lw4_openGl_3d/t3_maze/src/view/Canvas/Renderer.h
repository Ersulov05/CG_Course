#pragma once
#include <glad/glad.h>
#include <vector>
#include <unordered_map>
#include "./RendererData.h"
#include "./Shader/ShaderLoader.h"

class Renderer
{
public:
    Renderer()
    {
        m_oitEnabled = false;
    };

    void InitOIT(int width, int height, int maxFragments = 4 * 1024 * 1024)
    {
        m_oit.maxFragments = maxFragments;
        CreateOITBuffers(m_oit, width, height);
        m_oitEnabled = true;

        BindOITResources();

        CreateFullScreenQuad();
        m_resolveProgram = ShaderLoader::LoadShader("./shaders/resolve.vert", "./shaders/resolve.frag");
    }

    void ResizeOIT(int width, int height)
    {
        if (m_oit.width == width && m_oit.height == height)
            return;
        
        int maxFragments = m_oit.maxFragments;
        
        CleanupOIT();
        
        CreateOITBuffers(m_oit, width, height);
        m_oit.maxFragments = maxFragments;
        
        BindOITResources();
    }


    void DrawMesh(const MeshData &mesh)
    {
        if (!mesh.edgeVertices.empty() && !mesh.edgeIndices.empty() && mesh.edgeIndices.size() % 2 == 0)
        {
            DrawLines(mesh.edgeVertices, mesh.edgeIndices);
        }
        if (!mesh.vertices.empty() && !mesh.indices.empty() && mesh.indices.size() % 3 == 0)
        {
            DrawTriangles(mesh.vertices, mesh.indices);
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

    void ClearOITBuffers()
    {
        GLuint zero = 0;
        glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, m_oit.atomicCounter);
        glBufferSubData(GL_ATOMIC_COUNTER_BUFFER, 0, sizeof(GLuint), &zero);

        glBindTexture(GL_TEXTURE_2D, m_oit.headPointerTexture);
        GLuint clearValue = 0;
        glClearTexImage(m_oit.headPointerTexture, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, &clearValue);
    }

    void ResolveOIT(GLuint backgroundTexture, GLuint backgroundDepthTexture)
    {
        glUseProgram(m_resolveProgram);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, backgroundTexture);
        glUniform1i(glGetUniformLocation(m_resolveProgram, "uBackground"), 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, backgroundDepthTexture);
        glUniform1i(glGetUniformLocation(m_resolveProgram, "uBackgroundDepth"), 1);

        glBindVertexArray(m_fullScreenQuad.VAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);
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
        float px, py, pz; 
        float nx, ny, nz; 
        float r, g, b, a; 
    };

    std::unordered_map<size_t, GPUBuffer> m_triangleBuffers;
    std::unordered_map<size_t, GPUBuffer> m_lineBuffers;
    OITBuffers m_oit;
    bool m_oitEnabled;

    struct FullScreenQuad
    {
        GLuint VAO = 0;
        GLuint VBO = 0;
    } m_fullScreenQuad;
    GLuint m_resolveProgram = 0;

    void CreateFullScreenQuad()
    {
        float vertices[] = {
            -1.0f, -1.0f, 0.0f, 0.0f,
            1.0f, -1.0f, 1.0f, 0.0f,
            -1.0f, 1.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 1.0f};

        glGenVertexArrays(1, &m_fullScreenQuad.VAO);
        glGenBuffers(1, &m_fullScreenQuad.VBO);

        glBindVertexArray(m_fullScreenQuad.VAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_fullScreenQuad.VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);
    }

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

        glLineWidth(4.0f);
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
            glVertices.push_back(ToGLVertex(v));
        }

        glGenVertexArrays(1, &buffer.VAO);
        glBindVertexArray(buffer.VAO);

        glGenBuffers(1, &buffer.VBO);
        glBindBuffer(GL_ARRAY_BUFFER, buffer.VBO);
        glBufferData(GL_ARRAY_BUFFER,
                     glVertices.size() * sizeof(GLVertex),
                     glVertices.data(),
                     GL_STATIC_DRAW);

        glGenBuffers(1, &buffer.EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     indices.size() * sizeof(unsigned int),
                     indices.data(),
                     GL_STATIC_DRAW);

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

    void CreateOITBuffers(OITBuffers &oit, int width, int height)
    {
        oit.width = width;
        oit.height = height;

        // Head Pointer Texture
        glGenTextures(1, &oit.headPointerTexture);
        glBindTexture(GL_TEXTURE_2D, oit.headPointerTexture);
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_R32UI, width, height);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        // Fragment Buffer (SSBO)
        glGenBuffers(1, &oit.fragmentBuffer);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, oit.fragmentBuffer);
        glBufferData(GL_SHADER_STORAGE_BUFFER,
                     oit.maxFragments * sizeof(OITFragment),
                     nullptr,
                     GL_DYNAMIC_DRAW);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

        // Atomic Counter
        glGenBuffers(1, &oit.atomicCounter);
        glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, oit.atomicCounter);
        glBufferData(GL_ATOMIC_COUNTER_BUFFER, sizeof(GLuint), nullptr, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, 0);
    }

    void BindOITResources()
    {
        if (!m_oitEnabled)
            return;

        glBindImageTexture(0, m_oit.headPointerTexture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32UI);
        glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 1, m_oit.atomicCounter);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, m_oit.fragmentBuffer);

        GLenum err = glGetError();
        if (err != GL_NO_ERROR)
        {
            std::cout << "OpenGL error in BindOITResources: " << err << std::endl;
        }
    }

    void CleanupOIT()
    {
        if (m_oit.headPointerTexture)
            glDeleteTextures(1, &m_oit.headPointerTexture);
        if (m_oit.fragmentBuffer)
            glDeleteBuffers(1, &m_oit.fragmentBuffer);
        if (m_oit.atomicCounter)
            glDeleteBuffers(1, &m_oit.atomicCounter);
            
        m_oit.headPointerTexture = 0;
        m_oit.fragmentBuffer = 0;
        m_oit.atomicCounter = 0;
        m_oit.width = 0;
        m_oit.height = 0;
    }

    GLVertex ToGLVertex(Vertex v) {
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

        return glv;
    }
};