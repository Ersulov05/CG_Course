#pragma once
#include "./Canvas/ICanvas3D.h"
#include "../common/Geometry.h"

class MebiusTrip
{
public:
    MebiusTrip()
    {
        GenerateMebiusStrip();
    }

    void Draw(ICanvas3D &canvas)
    {

        canvas.DrawMesh(m_mesh);
    }

private:
    MeshData m_mesh;

    void GenerateMebiusStrip(
        float radius = 0.5f, 
        float width = 1.0f, 
        int segmentsU = 32, 
        int segmentsV = 8) 
    {
        m_mesh.subMeshes.clear();
        SubMeshData subMesh;
        
        for (int i = 0; i <= segmentsU; i++) {
            float u = (float(i) / segmentsU) * 2.0f * M_PI;
            float cosU = cos(u);
            float sinU = sin(u);
            float cosHalfU = cos(u * 0.5f);
            float sinHalfU = sin(u * 0.5f);

            float hue = u * 180 / M_PI;            
            Color color = Color::FromHSV(hue, 1, 1);            
            
            for (int j = 0; j <= segmentsV; j++) {
                float v = (float(j) / segmentsV) * width - width / 2;
                
                float r = radius + v/2 * cosHalfU;
                float x = r * cosU;
                float y = r * sinU;
                float z = v/2 * sinHalfU;
                
                Vector3D normal = GetMebiusPointNormal(r, v, sinU, cosU, sinHalfU, cosHalfU);
                
                subMesh.vertices.push_back(Vertex(Point3D(x, y, z), normal, color));
                subMesh.edgeVertices.push_back({Point3D(x, y, z), normal, Color(0x000000FF)});
            }
        }

        auto vCount = segmentsV + 1;
        for (int i = 0; i < segmentsU; i++) {
            for (int j = 0; j < segmentsV; j++) {
                int idx0 = j + i * vCount;
                int idx1 = idx0 + 1;      
                int idx2 = (i + 1) * vCount + j;
                int idx3 = idx2 + 1; 
                
                AddFaceIndices(subMesh, idx0, idx1, idx2, idx3);
                AddEdgeIndices(subMesh, idx0, idx1, idx2, idx3);
            }
        }
    }

    void AddFaceIndices(SubMeshData& subMesh, int idx0, int idx1, int idx2, int idx3) {
        subMesh.indices.push_back(idx0);
        subMesh.indices.push_back(idx1);
        subMesh.indices.push_back(idx2);
        
        subMesh.indices.push_back(idx1);
        subMesh.indices.push_back(idx2);
        subMesh.indices.push_back(idx3);
    }

    void AddEdgeIndices(SubMeshData& subMesh, int idx0, int idx1, int idx2, int idx3) {
        subMesh.edgeIndices.push_back(idx2);
        subMesh.edgeIndices.push_back(idx0);

        subMesh.edgeIndices.push_back(idx0);
        subMesh.edgeIndices.push_back(idx1);

        subMesh.edgeIndices.push_back(idx1);
        subMesh.edgeIndices.push_back(idx2);
        
        subMesh.edgeIndices.push_back(idx2);
        subMesh.edgeIndices.push_back(idx3);

        subMesh.edgeIndices.push_back(idx3);
        subMesh.edgeIndices.push_back(idx1);        
    }

    // x = (r + v/2 * cos(u/2)) * cos(u) | r * cos(u) + v/2 * cos(u/2) * cos(u)
    // y = (r + v/2 * cos(u/2)) * sin(u) | r * sin(u) + v/2 * cos(u/2) * sin(u)
    // z = v/2 * sin(u/2)
    Vector3D GetMebiusPointNormal(float r, float v, float sinU, float cosU, float sinHalfU, float cosHalfU){
        float dx_du = -r * sinU + (-0.5f * sinHalfU * cosU - sinU * cosHalfU) * 0.5f * v;
        float dy_du =  r * cosU + (-0.5f * sinHalfU * sinU + cosU * cosHalfU) * 0.5f * v;
        float dz_du =  0.5f * v * 0.5f * cosHalfU;
        
        float dx_dv = 0.5f * cosHalfU * cosU;
        float dy_dv = 0.5f * cosHalfU * sinU;
        float dz_dv = 0.5f * sinHalfU;
        
        Vector3D du(dx_du, dy_du, dz_du);
        Vector3D dv(dx_dv, dy_dv, dz_dv);
        return du.Cross(dv).Normalized();
    }
};