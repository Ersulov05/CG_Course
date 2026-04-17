#pragma once
#include "../Canvas/ICanvas3D.h"
#include "../Meshes/Mesh.h"
#include "../../controller/GameController.h"
#include "./HealthView.h"
#include "./CannonReloadView.h"

class UIView {
public:
    static void Draw(ICanvas3D& canvas, const GameController& controller)
    {
        auto& shaderManager = canvas.GetShaderManager();
        shaderManager.Push("ui");
        canvas.GetTransform().PushMatrix();
        canvas.GetTransform().ResetTransform();

        HealthView::Draw(canvas, 75, 100);

        auto cannon = controller.GetPlayerTank().GetCannon();
        CannonReloadView::Draw(canvas, cannon.GetReloadTime(), cannon.GetTotalReloadTime(), (int)cannon.GetMountType());

        canvas.GetTransform().PopMatrix();
        shaderManager.Pop();
    }
private:
};