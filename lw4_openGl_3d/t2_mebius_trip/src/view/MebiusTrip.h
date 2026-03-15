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
        float radius = 1.0f, 
        float width = 0.5f, 
        int segmentsU = 64, 
        int segmentsV = 16) 
    {
        m_mesh.vertices.clear();
        m_mesh.indices.clear();
        
        for (int i = 0; i <= segmentsU; i++) {
            float u = (float(i) / segmentsU) * 2.0f * M_PI;
            float cos_u = cos(u);
            float sin_u = sin(u);
            float cos_u_half = cos(u * 0.5f);
            float sin_u_half = sin(u * 0.5f);

            float hue = u * 180 / M_PI;            
            Color color = Color::FromHSV(hue, 1, 1);            
            
            for (int j = 0; j <= segmentsV; j++) {
                float v = (float(j) / segmentsV) * 2.0f * width - width;
                
                float r = radius + v * cos_u_half;
                float x = r * cos_u;
                float y = r * sin_u;
                float z = v * sin_u_half;
                
                Vector3D normal = GetMebiusPointNormal(r, v, sin_u, cos_u, sin_u_half, cos_u_half);
                
                m_mesh.vertices.push_back({Point3D(x, y, z), normal, color});
            }
        }

        for (int i = 0; i < segmentsU; i++) {
            for (int j = 0; j < segmentsV; j++) {
                // Индексы четырех вершин ячейки (i, j)
                // Вершины расположены в порядке:
                // (i, j) -> (i, j+1) -> (i+1, j+1) -> (i+1, j)
                int idx0 = i * (segmentsV + 1) + j;           // (i, j)
                int idx1 = i * (segmentsV + 1) + j + 1;       // (i, j+1)
                int idx2 = (i + 1) * (segmentsV + 1) + j + 1; // (i+1, j+1)
                int idx3 = (i + 1) * (segmentsV + 1) + j;     // (i+1, j)
                
                m_mesh.indices.push_back(idx0);
                m_mesh.indices.push_back(idx1);
                m_mesh.indices.push_back(idx2);
                
                m_mesh.indices.push_back(idx0);
                m_mesh.indices.push_back(idx2);
                m_mesh.indices.push_back(idx3);
            }
        }
    }

    Vector3D GetMebiusPointNormal(float r, float v, float sin_u, float cos_u, float sin_u_half, float cos_u_half){
        // dP/du
        float dx_du = -r * sin_u - 0.5f * v * cos_u * sin_u_half;
        float dy_du =  r * cos_u - 0.5f * v * sin_u * sin_u_half;
        float dz_du =  0.5f * v * cos_u_half;
        
        // dP/dv
        float dx_dv = cos_u_half * cos_u;
        float dy_dv = cos_u_half * sin_u;
        float dz_dv = sin_u_half;
        
        Vector3D du(dx_du, dy_du, dz_du);
        Vector3D dv(dx_dv, dy_dv, dz_dv);
        return du.Cross(dv).Normalized();
    }
};