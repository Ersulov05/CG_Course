#pragma once
#include "./Canvas/ICanvas3D.h"
#include "../model/Wall.h"

class WallView {
public:
    WallView(const Wall& wall): m_wall(wall) {
        Generate();
    }

    void Draw(ICanvas3D &canvas) {
        canvas.GetTransform().PushMatrix();
        auto pos = m_wall.GetPosition();
        auto size = m_wall.GetSize();
        canvas.GetTransform().Translate(pos.x, pos.y, pos.z);
        canvas.GetTransform().Scale(size.width, size.height, size.depth);
        canvas.DrawMesh(m_mesh);
        canvas.GetTransform().PopMatrix();
    }
private:
    MeshData m_mesh;
    const Wall &m_wall;

    void Generate() {
        auto color = m_wall.GetColor();
        auto size = m_wall.GetSize();

        Point3D positions[8] = {
            Point3D(-0.5, -0.5, -0.5), 
            Point3D(0.5, -0.5, -0.5),  
            Point3D(0.5, 0.5, -0.5),   
            Point3D(-0.5, 0.5, -0.5),  
            Point3D(-0.5, -0.5, 0.5),  
            Point3D(0.5, -0.5, 0.5),   
            Point3D(0.5, 0.5, 0.5),    
            Point3D(-0.5, 0.5, 0.5)    
        };

        Vector3D normals[6] = {
            Vector3D(0, 0, -1), // передняя
            Vector3D(0, 0, 1),  // задняя
            Vector3D(-1, 0, 0), // левая
            Vector3D(1, 0, 0),  // правая
            Vector3D(0, -1, 0), // нижняя
            Vector3D(0, 1, 0)   // верхняя
        };

        TextureCoord faceUV[4] = {
            TextureCoord(0, 0),
            TextureCoord(1, 0),
            TextureCoord(1, 1),
            TextureCoord(0, 1)
        };

        float faceUVSize[6][2] = {
            {size.width, size.height},
            {size.width, size.height},
            {size.depth, size.height},
            {size.depth, size.height},
            {size.width, size.depth},
            {size.width, size.depth}
        };

        int faceIndices[6][4] = {
            {0, 1, 2, 3}, // передняя
            {5, 4, 7, 6}, // задняя
            {4, 0, 3, 7}, // левая
            {1, 5, 6, 2}, // правая
            {4, 5, 1, 0}, // нижняя
            {3, 2, 6, 7}  // верхняя
        };

        for (int face = 0; face < 6; face++)
        {
            for (int i = 0; i < 4; i++)
            {
                TextureCoord uv = faceUV[i];
                uv.u *= faceUVSize[face][0];
                uv.v *= faceUVSize[face][1];
                int vertexIndex = faceIndices[face][i];
                m_mesh.vertices.push_back({positions[vertexIndex], normals[face], color, uv, faceUV[i]});
            }
        }

        for (int i = 0; i < 24; i += 4)
        {
            m_mesh.indices.push_back(i);
            m_mesh.indices.push_back(i + 1);
            m_mesh.indices.push_back(i + 2);

            m_mesh.indices.push_back(i);
            m_mesh.indices.push_back(i + 2);
            m_mesh.indices.push_back(i + 3);
        }

        m_mesh.textures = m_wall.GetTextures();
    }
};