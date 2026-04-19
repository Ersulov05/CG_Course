#pragma once
#include "../Canvas/ICanvas3D.h"
#include "../Meshes/Mesh.h"
#include "../../controller/GameController.h"
#include "../Components/BarView.h"

class HealthView {
public:
    static void Draw(ICanvas3D& canvas, float health, float totalHealth)
    {
        auto height = canvas.GetHeight();

        canvas.GetTransform().PushMatrix();
        canvas.GetTransform().Translate(HEALTH_BAR_WIDTH/2 + HEALTH_BAR_PADDING, height - HEALTH_BAR_HEIGHT/2 - HEALTH_BAR_PADDING, 0);

        BarData barData = {health, totalHealth};
        BarView::Draw(canvas, BAR_PARAM, barData);

        canvas.GetTransform().PopMatrix();
    }
private:
    inline static const float HEALTH_BAR_WIDTH = 200;
    inline static const float HEALTH_BAR_HEIGHT = 30;
    inline static const float HEALTH_BORDER_SIZE = 4;
    inline static const float HEALTH_BAR_PADDING = 20;

    inline static const BarParam BAR_PARAM = {HEALTH_BAR_WIDTH, HEALTH_BAR_HEIGHT, HEALTH_BORDER_SIZE, 0xFF0000FF, 0xFFFFFFFF};
};