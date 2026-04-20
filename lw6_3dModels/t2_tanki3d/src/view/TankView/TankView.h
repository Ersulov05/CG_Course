#pragma once
#include "../Canvas/ICanvas3D.h"
#include "../ModelLoader/ModelLoader.h"
#include "../../model/Tank/Tank.h"
#include "./CannonView.h"
#include "../Canvas/FpvCamera3D.h"
#include "../Components/BarView.h"
#include "../ObjectHealthView.h"
#include <stdexcept>

class TankView {
public:
    static void Draw(ICanvas3D& canvas, const std::shared_ptr<Tank>& tank, const FpvCamera3D& camera, bool isDrawHealth = false) 
    {
        canvas.GetTransform().PushMatrix();
        canvas.GetTransform().Translate(tank->GetPosition());
        canvas.GetTransform().Rotate(tank->GetRotation());

        canvas.DrawMesh(GetTankMesh(tank->GetType()));
        CannonView::Draw(canvas, tank->GetCannon());

        canvas.GetTransform().PopMatrix();

        if (isDrawHealth) {
            DrawHealth(canvas, tank, camera);
        }
    }
private:
    inline static const MeshData m_tankT34Model = ModelLoader::LoadModel("./models/t34/t34.obj");
    inline static const MeshData m_tankTigerModel = ModelLoader::LoadModel("./models/tiger1/tiger1.obj");
    inline static const float HEALTH_Y_OFFSET = 2;

    static void DrawHealth(ICanvas3D& canvas, const std::shared_ptr<Tank>& tank, const FpvCamera3D& camera)
    {
        auto healthPos = tank->GetPosition();
        healthPos.y += HEALTH_Y_OFFSET + tank->GetSize().height/2;
        BarData barData = {tank->GetHealth(), tank->GetTotalHealth()};
        ObjectHealthView::Draw(canvas, healthPos, camera, barData);
    }

    static MeshData GetTankMesh(TankType type)
    {
        switch (type) {
            case TankType::T34:
                return m_tankT34Model;
            case TankType::TIGER:
                return m_tankTigerModel;
            default:
                throw std::invalid_argument("Unknown Tank type: " + (int)type);
        }
    }
};