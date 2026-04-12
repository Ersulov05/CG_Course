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
        canvas.GetTransform().Scale(bonus.GetRadius() * 2);
        
        canvas.DrawMesh(m_bonusMesh);
        canvas.GetTransform().PopMatrix();
    }

private:
    inline static const MeshData m_bonusMesh = Mesh::Sphere(0xFFFF00FF);
};