#pragma once
#include "./Canvas/ICanvas3D.h"
#include "./ModelLoader/ModelLoader.h"
#include "../model/Map/Terrarian/Terrarian.h"
#include "./Meshes/Mesh.h"
#include "../model/Constants.h"

class TerrarianView {
public:
    static void Draw(ICanvas3D& canvas, const Terrarian& terrarian) 
    {
        canvas.GetTransform().PushMatrix();
        canvas.GetTransform().Translate(terrarian.GetPosition());
        canvas.GetTransform().Scale(terrarian.GetSize());

        auto terrarianMesh = GetTerrarianMeshByType(terrarian.GetType());
        canvas.DrawMesh(terrarianMesh);

        canvas.GetTransform().PopMatrix();
    }
private:
    inline static const MeshData m_terrarianDirtModel = Mesh::Cube(0xFFFFFFFF, {"./textures/grass.jpg"}, Constants::TERRARIAN_SIZE);
    inline static const MeshData m_terrarianIceModel = Mesh::Cube(0xFFFFFFFF, {"./textures/ice.jpg"}, Constants::TERRARIAN_SIZE);

    static MeshData GetTerrarianMeshByType(TerrarianType type) 
    {
        switch (type) {
            case TerrarianType::Dirt:
                return m_terrarianDirtModel;
            case TerrarianType::Ice:
                return m_terrarianIceModel;
            default:
                return m_terrarianDirtModel;
        }
    }
};