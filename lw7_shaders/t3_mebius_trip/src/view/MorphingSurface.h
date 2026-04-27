// MeshData.h
#pragma once
#include <vector>
#include "./Canvas/ICanvas3D.h"

class MorphingSurface {
public:
    MorphingSurface(int gridSize = 50) : m_gridSize(gridSize) {
        GenerateBaseGrid();
    }
    
    void Draw(ICanvas3D& canvas, float time) {
        // canvas.SetUniform("uTime", time);
        canvas.DrawMesh(m_mesh);
    }
    
private:
    MeshData m_mesh;
    int m_gridSize;
    
    void GenerateBaseGrid() {
        m_mesh.vertices.clear();
        m_mesh.indices.clear();
        
        // Генерируем вершины на плоскости XOY в диапазоне [-2, 2]
        float step = 4.0f / m_gridSize;
        
        for (int i = 0; i <= m_gridSize; i++) {
            float x = -2.0f + i * step;
            for (int j = 0; j <= m_gridSize; j++) {
                float y = -2.0f + j * step;

                float hue = fmod((x + 2.0f) * 45.0f + (y + 2.0f) * 45.0f, 360.0f);
                Color color = Color::FromHSV(hue, 0.9f, 1.0f);
                
                Vertex vertex;
                vertex.position = Point3D(x, y, 0.0f);
                vertex.normal = Vector3D(0,0,1);
                vertex.color = color;
                
                m_mesh.vertices.push_back(vertex);
                vertex.color = 0x000000FF;
                m_mesh.edgeVertices.push_back(vertex);
            }
        }
        
        int stride = m_gridSize + 1;
        for (int i = 0; i < m_gridSize; i++) {
            for (int j = 0; j < m_gridSize; j++) {
                int idx0 = i * stride + j;
                int idx1 = idx0 + 1;
                int idx2 = (i + 1) * stride + j;
                int idx3 = idx2 + 1;
                
                m_mesh.indices.push_back(idx0);
                m_mesh.indices.push_back(idx1);
                m_mesh.indices.push_back(idx2);
                
                m_mesh.indices.push_back(idx1);
                m_mesh.indices.push_back(idx3);
                m_mesh.indices.push_back(idx2);

                m_mesh.edgeIndices.push_back(idx0);
                m_mesh.edgeIndices.push_back(idx1);
                m_mesh.edgeIndices.push_back(idx1);
                m_mesh.edgeIndices.push_back(idx2);
                m_mesh.edgeIndices.push_back(idx2);
                m_mesh.edgeIndices.push_back(idx0);
            }
        }
    }
};