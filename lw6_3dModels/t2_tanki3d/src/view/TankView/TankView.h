#pragma once
#include "../Canvas/ICanvas3D.h"
#include "../ModelLoader/ModelLoader.h"
#include "../../model/Tank/Tank.h"
#include "./CannonView.h"
#include "../Canvas/FpvCamera3D.h"
#include "../Components/BarView.h"

class TankView {
public:
    static void Draw(ICanvas3D& canvas, const std::shared_ptr<Tank>& tank, const FpvCamera3D& camera, bool isDrawHealth = false) 
    {
        canvas.GetTransform().PushMatrix();
        canvas.GetTransform().Translate(tank->GetPosition());
        if (isDrawHealth) {
            DrawHealth(canvas, tank, camera);
        }
        
        canvas.GetTransform().Rotate(tank->GetRotation());
        canvas.DrawMesh(m_tankModel);
        CannonView::Draw(canvas, tank->GetCannon());
        canvas.GetTransform().PopMatrix();
    }
private:
    inline static const MeshData m_tankModel = ModelLoader::LoadModel("./models/t34/t34.obj");
    inline static const BarParam BAR_PARAM = {4, 0.4, 0.1, 0xFF00008F, 0xFFFFFF8F};
    inline static const float HEALTH_Y_OFFSET = 2;

    static void DrawHealth(ICanvas3D& canvas, const std::shared_ptr<Tank>& tank, const FpvCamera3D& camera)
    {
        BarData barData = {tank->GetHealth(), tank->GetTotalHealth()};
        canvas.GetTransform().PushMatrix();
        canvas.GetTransform().Translate({0, tank->GetSize().height/2 + HEALTH_Y_OFFSET, 0});

        Quaternion3D rotationToCamera = GetRotationToCamera(camera.GetPosition(), tank->GetPosition());
        canvas.GetTransform().Rotate(rotationToCamera);

        BarView::Draw(canvas, BAR_PARAM, barData);
        canvas.GetTransform().PopMatrix();
    }

    static Quaternion3D GetYRotationToCamera(const Point3D& cameraPos, const Point3D& objectPos)
    {
        Vector3D direction = cameraPos - objectPos;
        direction.Normalized();
        
        float yaw = atan2(direction.x, direction.z);
        return Quaternion3D(0, yaw, 0);
    }

    static Quaternion3D GetRotationToCamera(const Point3D& cameraPos, const Point3D& objectPos)
    {
        Vector3D direction = cameraPos - objectPos;
        direction.Normalized();
        
        return Quaternion3D::FromDirection(direction);
    }
};