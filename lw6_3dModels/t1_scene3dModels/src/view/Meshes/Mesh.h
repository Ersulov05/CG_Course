#pragma once
#include "../../common/Geometry.h"
#include "../Canvas/ICanvas3D.h"

class Mesh {
public:
    static MeshData Cube(Color color, std::vector<std::string> textures = {}, Size3D size = {1, 1, 1}) {
        MeshData meshData;
        SubMeshData mesh;

        float faceUVSize[6][2] = {
            {size.width, size.height},
            {size.width, size.height},
            {size.depth, size.height},
            {size.depth, size.height},
            {size.width, size.depth},
            {size.width, size.depth}
        };

        TextureCoord uv = {0, 0};
        for (int face = 0; face < 6; face++)
        {
            for (int i = 0; i < 4; i++)
            {
                TextureCoord uv = CUBE_FACE_UV[i];
                uv.u *= faceUVSize[face][0];
                uv.v *= faceUVSize[face][1];

                int vertexIndex = CUBE_FACE_INDICES[face][i];
                mesh.vertices.push_back({CUBE_VERTEX_POSITIONS[vertexIndex], CUBE_FACE_NORMALS[face], color, uv, CUBE_FACE_UV[i]});
            }
        }

        for (int i = 0; i < 24; i += 4)
        {
            mesh.indices.push_back(i);
            mesh.indices.push_back(i + 1);
            mesh.indices.push_back(i + 2);

            mesh.indices.push_back(i);
            mesh.indices.push_back(i + 2);
            mesh.indices.push_back(i + 3);
        }

        mesh.textures = textures;
        meshData.subMeshes.push_back(mesh);
        return meshData;
    }

    static MeshData Sphere(Color color, std::vector<std::string> textures = {}, float radius = 0.5f, int rings = 32, int sectors = 32) {
        MeshData meshData;
        SubMeshData mesh;

        float const R = 1.0f / (float)(rings - 1);
        float const S = 1.0f / (float)(sectors - 1);
        
        for (int r = 0; r < rings; ++r) {
            float phi = M_PI * r * R;
            float sinPhi = sin(phi);
            float cosPhi = cos(phi);
            
            for (int s = 0; s < sectors; ++s) {
                float theta = 2 * M_PI * s * S;
                float sinTheta = sin(theta);
                float cosTheta = cos(theta);
                
                float x = sinPhi * cosTheta;
                float z = sinPhi * sinTheta;
                float y = cosPhi;
                
                Point3D position(x * radius, y * radius, z * radius);
                
                Vector3D normal(x, y, z);
                normal.Normalized();
                
                TextureCoord uv = {s * S, r * R};
                
                mesh.vertices.push_back({position, normal, color, uv, uv});
            }
        }
        
        for (int r = 0; r < rings - 1; ++r) {
            for (int s = 0; s < sectors - 1; ++s) {
                int current = r * sectors + s;
                int next = (r + 1) * sectors + s;
                
                mesh.indices.push_back(current);
                mesh.indices.push_back(current + 1);
                mesh.indices.push_back(next);
                
                mesh.indices.push_back(next);
                mesh.indices.push_back(current + 1);
                mesh.indices.push_back(next + 1);
            }
        }

        mesh.textures = textures;

        meshData.subMeshes.push_back(mesh);
        return meshData;
    }
private:
    inline static Point3D CUBE_VERTEX_POSITIONS[8] = {
        Point3D(-0.5, -0.5, -0.5), 
        Point3D(0.5, -0.5, -0.5),  
        Point3D(0.5, 0.5, -0.5),   
        Point3D(-0.5, 0.5, -0.5),  
        Point3D(-0.5, -0.5, 0.5),  
        Point3D(0.5, -0.5, 0.5),   
        Point3D(0.5, 0.5, 0.5),    
        Point3D(-0.5, 0.5, 0.5)  
    };

    inline static Vector3D CUBE_FACE_NORMALS[6] = {
        Vector3D(0, 0, -1), // передняя
        Vector3D(0, 0, 1),  // задняя
        Vector3D(-1, 0, 0), // левая
        Vector3D(1, 0, 0),  // правая
        Vector3D(0, -1, 0), // нижняя
        Vector3D(0, 1, 0)   // верхняя
    };

    inline static int CUBE_FACE_INDICES[6][4] = {
        {0, 1, 2, 3}, // передняя
        {5, 4, 7, 6}, // задняя
        {4, 0, 3, 7}, // левая
        {1, 5, 6, 2}, // правая
        {4, 5, 1, 0}, // нижняя
        {3, 2, 6, 7}  // верхняя
    };

    inline static TextureCoord CUBE_FACE_UV[4] = {
        TextureCoord(0, 0),
        TextureCoord(1, 0),
        TextureCoord(1, 1),
        TextureCoord(0, 1)
    };
};