#pragma once
#include "../model/Bonus/Bonus.h"
#include "./Canvas/ICanvas3D.h"
#include "./Meshes/Mesh.h"

class BonusView {
public:
    static void Draw(const Bonus& bonus, ICanvas3D &canvas) {
        canvas.GetTransform().PushMatrix();
        canvas.GetTransform().Translate(bonus.GetPosition());
        canvas.GetTransform().Translate(0, bonus.GetRadius(), 0);
        auto scale = bonus.GetRadius() * 2;
        canvas.GetTransform().Scale(scale * 1.5, scale, scale);
        
        canvas.DrawMesh(m_bonusMesh);
        canvas.GetTransform().PopMatrix();
    }

private:
    inline static const MeshData m_bonusMesh = Mesh::Sphere(0xFFFFFFFF, {"./textures/bonus.jpg"});
};