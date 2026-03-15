#pragma once
#include "../../common/Color.h"
#include "../../common/Point3D.h"
#include "../../common/Vector3D.h"
#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>

struct Vertex
{
    Point3D position;
    Vector3D normal;
    Color color;
};

struct MeshData
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Vertex> edgeVertices;
    std::vector<unsigned int> edgeIndices;
};

namespace std
{
    template <>
    struct hash<Vertex>
    {
        size_t operator()(const Vertex &v) const noexcept
        {
            size_t h1 = hash<float>()(v.position.x);
            size_t h2 = hash<float>()(v.position.y);
            size_t h3 = hash<float>()(v.position.z);
            size_t h4 = hash<float>()(v.normal.x);
            size_t h5 = hash<float>()(v.normal.y);
            size_t h6 = hash<float>()(v.normal.z);
            size_t h7 = hash<float>()(v.color.r);
            size_t h8 = hash<float>()(v.color.g);
            size_t h9 = hash<float>()(v.color.b);
            size_t h10 = hash<float>()(v.color.a);

            return h1 ^ (h2 << 1) ^ (h3 << 2) ^ (h4 << 3) ^
                   (h5 << 4) ^ (h6 << 5) ^ (h7 << 6) ^
                   (h8 << 7) ^ (h9 << 8) ^ (h10 << 9);
        }
    };
}

struct OITBuffers
{
    GLuint headPointerTexture = 0;      // текстура с указателями (1 указатель на пиксель)
    GLuint fragmentBuffer = 0;          // SSBO для хранения фрагментов
    GLuint atomicCounter = 0;           // атомарный счётчик
    int maxFragments = 4 * 1024 * 1024; // максимум фрагментов (4 млн)
    int width = 0, height = 0;
};

struct OITFragment
{
    glm::vec4 color;
    float depth;
    uint next;
};
