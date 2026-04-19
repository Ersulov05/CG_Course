#pragma once
#include "./Canvas/ICanvas3D.h"
#include "./ModelLoader/ModelLoader.h"
#include "./Canvas/FpvCamera3D.h"
#include "./Components/BarView.h"

class ObjectHealthView {
public:
    static void Draw(ICanvas3D& canvas, const Point3D& position, const FpvCamera3D& camera, const BarData& barData) 
    {        
        Quaternion3D rotationToCamera = GetRotationToCamera(camera.GetPosition(), position);

        canvas.GetTransform().PushMatrix();
        canvas.GetTransform().Translate(position);
        canvas.GetTransform().Rotate(rotationToCamera);

        BarView::Draw(canvas, BAR_PARAM, barData);
        canvas.GetTransform().PopMatrix();
    }
private:
    inline static const BarParam BAR_PARAM = {4, 0.3, 0.08, 0xFF0000AF, 0xFFFFFFAF};

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