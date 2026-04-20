#pragma once
#include "../Canvas/ICanvas3D.h"
#include "../Meshes/Mesh.h"
#include "../../model/Game.h"
#include "../Components/BarView.h"

class ProgressView {
public:
    static void Draw(ICanvas3D& canvas, int progress)
    {
        canvas.GetTransform().PushMatrix();
        canvas.GetTransform().Translate(BAR_WIDTH/2 + BAR_PADDING, BAR_HEIGHT/2 + BAR_PADDING, 0);
        
        BarData barData = {progress, Game::WIN_KILL_COUNT};
        BarView::Draw(canvas, BAR_PARAM, barData);
        
        canvas.GetTransform().PopMatrix();
    }
private:
    inline static const float BAR_WIDTH = 350;
    inline static const float BAR_HEIGHT = 35;
    inline static const float BORDER_SIZE = 5;
    inline static const float BAR_PADDING = 20;
    inline static const BarParam BAR_PARAM = {BAR_WIDTH, BAR_HEIGHT, BORDER_SIZE, 0xA811A6FF, 0xFFFFFFFF};
};