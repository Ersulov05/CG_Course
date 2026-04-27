#pragma once
#include <vector>
#include "./Canvas/ICanvas3D.h"
#include "./Canvas/Shader/BasicShader.h"

class Surface {
public:
    Surface(int gridSize = 32) : m_gridSize(gridSize) {
        GenerateSurface();
    }
    
    void Draw(ICanvas3D& canvas, float time) {

        auto& shaderManager = canvas.GetShaderManager();
        auto* basicShader = dynamic_cast<BasicShader*>(shaderManager.GetByName("basic"));
        if (!basicShader) return;

        basicShader->SetTime(time);
        canvas.DrawMesh(m_mesh);
    }
    
private:
    MeshData m_mesh;
    int m_gridSize;
    
    void GenerateSurface() {
        m_mesh.subMeshes.clear();
        SubMeshData subMesh;
        Color color = 0x0000FFFF;
        
        float step = 2.0f / m_gridSize;
        
        for (int i = 0; i <= m_gridSize; i++) {
            float x = -1.0f + i * step;
            for (int j = 0; j <= m_gridSize; j++) {
                float y = -1.0f + j * step;
                
                Vertex vertex;
                vertex.position = Point3D(x, y, 0.0f);
                vertex.normal = Vector3D(0,0,1);
                vertex.color = color;
                
                subMesh.vertices.push_back(vertex);
            }
        }
        
        int stride = m_gridSize + 1;
        for (int i = 0; i < m_gridSize; i++) {
            for (int j = 0; j < m_gridSize; j++) {
                int idx0 = i * stride + j;
                int idx1 = idx0 + 1;
                int idx2 = (i + 1) * stride + j;
                int idx3 = idx2 + 1;
                
                subMesh.indices.push_back(idx0);
                subMesh.indices.push_back(idx1);
                subMesh.indices.push_back(idx2);
                
                subMesh.indices.push_back(idx1);
                subMesh.indices.push_back(idx3);
                subMesh.indices.push_back(idx2);
            }
        }

        m_mesh.subMeshes.push_back(subMesh);
    }
};