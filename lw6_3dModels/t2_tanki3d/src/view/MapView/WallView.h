#pragma once
#include "../Canvas/ICanvas3D.h"
#include "../ModelLoader/ModelLoader.h"
#include "../../model/Map/Wall/Wall.h"
#include "../Meshes/Mesh.h"
#include "../../model/Constants.h"

class WallView {
public:
    static void Draw(ICanvas3D& canvas, const Wall& wall) 
    {
        if (wall.GetType() == WallType::Empty) 
        {
            return;
        }
        canvas.GetTransform().PushMatrix();
        canvas.GetTransform().Translate(wall.GetPosition());
        canvas.GetTransform().Scale(wall.GetSize());

        auto wallMesh = GetWallMeshByType(wall.GetType());
        canvas.DrawMesh(wallMesh);

        canvas.GetTransform().PopMatrix();
    }
private:
    inline static const MeshData m_wallBrickModel = Mesh::Cube(0xFFFFFFFF, {"./textures/brick.jpg"}, Constants::DEFAULT_WALL_SIZE/4);
    inline static const MeshData m_wallSteelModel = Mesh::Cube(0xFFFFFFFF, {"./textures/steel_glass.jpg"});

    static MeshData GetWallMeshByType(WallType type) 
    {
        switch (type) {
            case WallType::Brick:
                return m_wallBrickModel;
            case WallType::Steel:
                return m_wallSteelModel;
            default:
                return m_wallBrickModel;
        }
    }
};