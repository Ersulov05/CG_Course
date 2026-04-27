#pragma once
#include "../../common/Color.h"
#include "../../common/Point3D.h"
#include "../../common/Vector3D.h"
#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>

struct TextureCoord 
{
    float u = 0;
    float v = 0;

    TextureCoord() {}
    TextureCoord(float u, float v): u(u), v(v) {}
};

struct Vertex
{
    Point3D position;
    Vector3D normal;
    Color color;
    TextureCoord uvDiffuse;
    TextureCoord uvShadow;
};

struct SubMeshData
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Vertex> edgeVertices;
    std::vector<unsigned int> edgeIndices;
    std::vector<std::string> textures;
};

struct MeshData
{
    std::vector<SubMeshData> subMeshes;
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
            size_t h11 = hash<float>()(v.uvDiffuse.u);
            size_t h12 = hash<float>()(v.uvDiffuse.v);
            size_t h13 = hash<float>()(v.uvShadow.u);
            size_t h14 = hash<float>()(v.uvShadow.v);

            return h1 ^ (h2 << 1) ^ (h3 << 2) ^ (h4 << 3) ^
                   (h5 << 4) ^ (h6 << 5) ^ (h7 << 6) ^
                   (h8 << 7) ^ (h9 << 8) ^ (h10 << 9) ^ 
                   (h11 << 10) ^ (h12 << 11) ^ (h13 << 12) ^ (h14 << 13);
        }
    };
}

struct OITBuffers
{
    GLuint headPointerTexture = 0;
    GLuint fragmentBuffer = 0;          
    GLuint atomicCounter = 0;           
    int maxFragments = 4 * 1024 * 1024;
    int width = 0, height = 0;
};

struct OITFragment
{
    glm::vec4 color;
    float depth;
    uint next;
};
