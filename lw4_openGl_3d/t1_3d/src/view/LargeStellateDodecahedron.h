#pragma once
#include "./Canvas/ICanvas3D.h"
#include "../common/Geometry.h"

class LargeStellateDodecahedron
{
public:
    LargeStellateDodecahedron()
    {
        GenerateDodecahedron();
    }
    void Draw(ICanvas3D &canvas)
    {
        canvas.DrawMesh(m_mesh);
    }

private:
    MeshData m_mesh;
    Color m_colors[20] = {
        Color(0xFF00004F), // Красный
        Color(0x00FF004F), // Зеленый
        Color(0x0000FF4F), // Синий
        Color(0xFFFF004F), // Желтый
        Color(0xFF00FF4F), // Пурпурный
        Color(0x00FFFF4F), // Голубой
        Color(0xFF8000FF), // Оранжевый
        Color(0xFF0080FF), // Розовый
        Color(0x80FF00FF), // Салатовый
        Color(0x00FF80FF), // Мятный
        Color(0x0080FFFF), // Небесно-голубой
        Color(0x8000FFFF), // Фиолетовый
        Color(0x800000FF), // Темно-красный
        Color(0x008000FF), // Темно-зеленый
        Color(0x000080FF), // Темно-синий
        Color(0x808000FF), // Оливковый
        Color(0x800080FF), // Пурпурный
        Color(0x008080FF), // Бирюзовый
        Color(0xFF8080FF), // Светло-розовый
        Color(0x80FF80FF), // Светло-зеленый
    };

    struct DodecahedronPiramides
    {
        Point3D vertices[32];
        int indices[20][4];
    };

    struct IkosaedrData
    {
        Point3D vertices[12];
        int faces[20][3];
    };

    void GenerateDodecahedron()
    {
        DodecahedronPiramides dPiramidesData = GetDodecahedronPiramides();

        m_mesh.vertices.clear();
        m_mesh.indices.clear();

        for (int i = 0; i < 20; i++)
        {
            Point3D apex = dPiramidesData.vertices[dPiramidesData.indices[i][0]];
            Point3D v0 = dPiramidesData.vertices[dPiramidesData.indices[i][1]];
            Point3D v1 = dPiramidesData.vertices[dPiramidesData.indices[i][2]];
            Point3D v2 = dPiramidesData.vertices[dPiramidesData.indices[i][3]];
            Color faceColor = m_colors[i % 20];

            AddDodecahedronFaces(apex, v0, v1, v2, faceColor);
            AddAddDodecahedronEdges(apex, v0, v1, v2, Color(0x000000FF));
        }
    }

    void AddDodecahedronFaces(Point3D apex, Point3D v0, Point3D v1, Point3D v2, Color color) {
        
        const Vector3D normal;
        int offset = m_mesh.vertices.size();
        
        m_mesh.vertices.push_back({apex, normal, color});
        m_mesh.vertices.push_back({v0, normal, color});
        m_mesh.vertices.push_back({v1, normal, color});

        m_mesh.vertices.push_back({apex, normal, color});
        m_mesh.vertices.push_back({v1, normal, color});
        m_mesh.vertices.push_back({v2, normal, color});

        m_mesh.vertices.push_back({apex, normal, color});
        m_mesh.vertices.push_back({v2, normal, color});
        m_mesh.vertices.push_back({v0, normal, color});

        for (int j = 0; j < 3; j++)
        {
            m_mesh.indices.push_back(offset + j * 3 + 0);
            m_mesh.indices.push_back(offset + j * 3 + 1);
            m_mesh.indices.push_back(offset + j * 3 + 2);
        }
    }

    void AddAddDodecahedronEdges(Point3D apex, Point3D v0, Point3D v1, Point3D v2, Color color) {
        
        const Vector3D normal;
        int edgeOffset = m_mesh.edgeVertices.size();

        m_mesh.edgeVertices.push_back({v0, normal, color});
        m_mesh.edgeVertices.push_back({v1, normal, color});
        m_mesh.edgeVertices.push_back({v2, normal, color});
        m_mesh.edgeVertices.push_back({apex, normal, color});

        for (int j = 0; j < 3; j++)
        {
            m_mesh.edgeIndices.push_back(edgeOffset + j);
            m_mesh.edgeIndices.push_back(edgeOffset + (j + 1) % 3);
            m_mesh.edgeIndices.push_back(edgeOffset + j);
            m_mesh.edgeIndices.push_back(edgeOffset + 3);
        }
    }

    IkosaedrData GetIkosaedr()
    {
        IkosaedrData result;

        const float phi = (1.0f + std::sqrt(5.0f)) / 2.0f;
        float scale = 1.0f / std::sqrt(1.0f + phi * phi);

        result.vertices[0] = Point3D(0, 1, phi) * scale;
        result.vertices[1] = Point3D(0, 1, -phi) * scale;
        result.vertices[2] = Point3D(0, -1, phi) * scale;
        result.vertices[3] = Point3D(0, -1, -phi) * scale;

        result.vertices[4] = Point3D(phi, 0, 1) * scale;
        result.vertices[5] = Point3D(phi, 0, -1) * scale;
        result.vertices[6] = Point3D(-phi, 0, 1) * scale;
        result.vertices[7] = Point3D(-phi, 0, -1) * scale;

        result.vertices[8] = Point3D(1, phi, 0) * scale;
        result.vertices[9] = Point3D(-1, phi, 0) * scale;
        result.vertices[10] = Point3D(1, -phi, 0) * scale;
        result.vertices[11] = Point3D(-1, -phi, 0) * scale;

        int faces[20][3] = {
            {8, 4, 5},
            {9, 6, 7},

            {8, 9, 0}, // v
            {8, 9, 1}, // v

            {8, 4, 0},
            {8, 5, 1},
            {9, 6, 0},
            {9, 7, 1},

            {10, 4, 5},
            {11, 6, 7},

            {10, 11, 2}, // n
            {10, 11, 3}, // n

            {10, 2, 4},
            {11, 2, 6},
            {10, 3, 5},
            {11, 3, 7},

            {0, 2, 4},
            {0, 2, 6},
            {1, 3, 5},
            {1, 3, 7}};

        for (int i = 0; i < 20; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                result.faces[i][j] = faces[i][j];
            }
        }

        return result;
    }

    DodecahedronPiramides GetDodecahedronPiramides()
    {
        DodecahedronPiramides result;
        IkosaedrData iko = GetIkosaedr();
        const int ikoVertexCount = 12;

        for (int i = 0; i < ikoVertexCount; i++)
        {
            result.vertices[i] = iko.vertices[i];
        }

        const float pyramidHeight = 3.0f;

        for (int face = 0; face < 20; face++)
        {
            int i0 = iko.faces[face][0];
            int i1 = iko.faces[face][1];
            int i2 = iko.faces[face][2];

            Point3D center = (iko.vertices[i0] + iko.vertices[i1] + iko.vertices[i2]) / 3.0f;
            Vector3D normal = ToVector(center).Normalized();
            Point3D apex = ToPoint(normal * pyramidHeight);

            int iApex = ikoVertexCount + face;
            result.vertices[iApex] = apex;

            result.indices[face][0] = iApex;
            result.indices[face][1] = i0;
            result.indices[face][2] = i1;
            result.indices[face][3] = i2;
        }

        return result;
    }
};