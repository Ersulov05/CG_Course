#pragma once
#include "../Canvas/ICanvas3D.h"
#include "../Meshes/Mesh.h"
#include "../../controller/GameController.h"
#include "./HealthView.h"
#include "./CannonReloadView.h"
#include "./GameStateView.h"
#include "./ProgressView.h"

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
        GameStateView::Draw(canvas, controller.GetGameState(), controller.GetReloadTime());
        ProgressView::Draw(canvas, controller.GetKillCount());

        canvas.GetTransform().PopMatrix();
        shaderManager.Pop();
    }
private:
};