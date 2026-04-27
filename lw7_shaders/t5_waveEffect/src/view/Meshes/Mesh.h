#pragma once
#include "../../common/Geometry.h"
#include "../Canvas/ICanvas3D.h"

class Mesh {
public:
    static MeshData Rect(Color color, const std::vector<std::string>& textures = {})
    {
        MeshData meshData;
        SubMeshData mesh;

        TextureCoord uv = {0, 0};
        for (int i = 0; i < 4; i++)
        {
            TextureCoord uv = RECT_FACE_UV[i];

            int vertexIndex = RECT_FACE_INDICES[i];
            mesh.vertices.push_back({RECT_VERTEX_POSITIONS[vertexIndex], {0, 0, 1}, color, uv, uv});
        }

        mesh.indices.push_back(0);
        mesh.indices.push_back(1);
        mesh.indices.push_back(2);

        mesh.indices.push_back(0);
        mesh.indices.push_back(2);
        mesh.indices.push_back(3);

        mesh.textures = textures;

        meshData.subMeshes.push_back(mesh);
        return meshData;   
    }

private:
    inline static Point3D RECT_VERTEX_POSITIONS[4] = {
        Point3D(-0.5, -0.5, 0), 
        Point3D(0.5, -0.5, 0),  
        Point3D(0.5, 0.5, 0),   
        Point3D(-0.5, 0.5, 0)  
    };

    inline static int RECT_FACE_INDICES[4] = {0, 1, 2, 3};

    inline static TextureCoord RECT_FACE_UV[4] = {
        TextureCoord(0, 0),
        TextureCoord(1, 0),
        TextureCoord(1, 1),
        TextureCoord(0, 1)
    };
};