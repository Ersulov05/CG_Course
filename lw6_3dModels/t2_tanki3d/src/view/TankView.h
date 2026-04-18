#pragma once
#include "./Canvas/ICanvas3D.h"
#include "./ModelLoader/ModelLoader.h"
#include "../model/Tank/Tank.h"
#include "./CannonView.h"

class TankView {
public:
    static void Draw(ICanvas3D& canvas, const std::shared_ptr<Tank>& tank, bool isEnemy = false) 
    {
        canvas.GetTransform().PushMatrix();
        canvas.GetTransform().Translate(tank->GetPosition());
        canvas.GetTransform().Rotate(tank->GetRotation());
        canvas.DrawMesh(m_tankModel);
        CannonView::Draw(canvas, tank->GetCannon());
        canvas.GetTransform().PopMatrix();
    }
private:
    inline static const MeshData m_tankModel = ModelLoader::LoadModel("./models/t34/t34.obj");
    // inline static const MeshData m_tankModel = ModelLoader::LoadModel("./models/car01/car01.obj");
};