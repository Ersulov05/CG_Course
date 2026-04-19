#pragma once
#include "../Canvas/ICanvas3D.h"
#include "../Meshes/Mesh.h"
#include "../../controller/GameController.h"
#include "../Components/BarView.h"

class CannonReloadView {
public:
    static void Draw(ICanvas3D& canvas, float reloadTime, float totalReloadTime, int count)
    {
        auto height = canvas.GetHeight();
        auto width = canvas.GetWidth();

        canvas.GetTransform().PushMatrix();
        canvas.GetTransform().Translate(width - RELOAD_BAR_WIDTH/2 - RELOAD_BAR_PADDING, height - RELOAD_BAR_HEIGHT/2 - RELOAD_BAR_PADDING, 0);
        canvas.GetTransform().RotateZ(-90);

        BarData barData = {totalReloadTime - reloadTime, totalReloadTime};
        for (int i = 0; i < count; ++i)
        {
            BarView::Draw(canvas, BAR_PARAM, barData);
            canvas.GetTransform().Translate(-RELOAD_BAR_WIDTH - OFFSET, 0, 0);
        }

        canvas.GetTransform().PopMatrix();
    }
private:
    inline static const MeshData m_healthRect = Mesh::Rect(0xBFBF00FF);
    inline static const MeshData m_healthBorderRect = Mesh::Rect(0xFFFFFFFF);
    inline static const float RELOAD_BAR_WIDTH = 30;
    inline static const float RELOAD_BAR_HEIGHT = 70;
    inline static const float RELOAD_BORDER_SIZE = 4;
    inline static const float RELOAD_BAR_PADDING = 20;
    inline static const float OFFSET = 15;

    inline static const BarParam BAR_PARAM = {RELOAD_BAR_HEIGHT, RELOAD_BAR_WIDTH, RELOAD_BORDER_SIZE, 0xBFBF00FF, 0xFFFFFFFF};
};