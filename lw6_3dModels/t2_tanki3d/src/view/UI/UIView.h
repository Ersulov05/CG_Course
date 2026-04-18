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
        auto playerTank = controller.GetPlayerTank();
        auto& cannon = playerTank->GetCannon();

        auto& shaderManager = canvas.GetShaderManager();
        shaderManager.Push("ui");
        canvas.GetTransform().PushMatrix();
        canvas.GetTransform().ResetTransform();
        
        HealthView::Draw(canvas, playerTank->GetHealth(), playerTank->GetTotalHealth());
        CannonReloadView::Draw(canvas, cannon.GetReloadTime(), cannon.GetTotalReloadTime(), (int)cannon.GetMountType());

        canvas.GetTransform().PopMatrix();
        shaderManager.Pop();
    }
private:
};