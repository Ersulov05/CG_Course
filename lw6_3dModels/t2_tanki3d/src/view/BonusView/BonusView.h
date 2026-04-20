#pragma once
#include "../Canvas/ICanvas3D.h"
#include "../ModelLoader/ModelLoader.h"
#include "../../model/Bonus/Bonus.h"
#include "../Meshes/Mesh.h"
#include "../../model/Constants.h"
#include <unordered_map>

class BonusView {
public:
    static void Draw(ICanvas3D& canvas, const Bonus& bonus)
    {
        canvas.GetTransform().PushMatrix();
        canvas.GetTransform().Translate(bonus.GetPosition());
        canvas.GetTransform().Scale(bonus.GetSize());

        auto bonusMesh = GetMeshByBonusType(bonus.GetType());
        canvas.DrawMesh(bonusMesh);

        canvas.GetTransform().PopMatrix();
    }
private:
    inline static const std::unordered_map<BonusActionType, MeshData> m_bonusMeshes = {
        {BonusActionType::Star, Mesh::Cube(0xFF0000FF, {"./textures/wood_box.jpg"})},
        {BonusActionType::Shovel, Mesh::Cube(0xFFFF00FF, {"./textures/wood_box.jpg"})},
        {BonusActionType::Medkit, Mesh::Cube(0x00FF00FF, {"./textures/wood_box.jpg"})},
        {BonusActionType::Machingan, Mesh::Cube(0x555555FF, {"./textures/wood_box.jpg"})},
        {BonusActionType::Helmet, Mesh::Cube(0x00FFFFFF, {"./textures/wood_box.jpg"})},
        {BonusActionType::Freeze, Mesh::Cube(0xFFFFFFFF, {"./textures/wood_box.jpg"})},
    };

    static MeshData GetMeshByBonusType(BonusActionType type) {
        auto it = m_bonusMeshes.find(type);
        
        if (it != m_bonusMeshes.end()) {
            return it->second;
        }
        
        static const MeshData defaultMesh = Mesh::Cube(0xFFFFFFFF, {"./textures/wood_box.jpg"});
        return defaultMesh;
    };
};