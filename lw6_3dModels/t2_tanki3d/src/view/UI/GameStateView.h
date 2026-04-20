#pragma once
#include "../Canvas/ICanvas3D.h"
#include "../Meshes/Mesh.h"
#include "../../model/Game.h"
#include "../Components/BarView.h"

class GameStateView {
public:
    static void Draw(ICanvas3D& canvas, GameState state, float reloadTime)
    {
        if (state == GameState::Play) return;
        canvas.GetTransform().PushMatrix();
        canvas.GetTransform().Translate(canvas.GetWidth()/2, canvas.GetHeight()/2, 0);
        
        canvas.GetTransform().PushMatrix();
        canvas.GetTransform().RotateZ(180);
        canvas.GetTransform().Scale({500, 500, 1});
        if (state == GameState::Win) {
            canvas.DrawMesh(m_gameWin);
        } else {
            canvas.DrawMesh(m_gameOver);
        }
        canvas.GetTransform().PopMatrix();

        canvas.GetTransform().Translate(0, 300, 0);
        BarData barData = {Game::TOTAL_RELOAD_TIME - reloadTime, Game::TOTAL_RELOAD_TIME};
        BarView::Draw(canvas, BAR_PARAM, barData);
        
        canvas.GetTransform().PopMatrix();
    }
private:
    inline static const MeshData m_gameOver = Mesh::Cube(0xFFFFFFFF, {"./textures/game_over.png"});
    inline static const MeshData m_gameWin = Mesh::Cube(0xFFFFFFFF, {"./textures/game_win.png"});
    inline static const float BAR_WIDTH = 350;
    inline static const float BAR_HEIGHT = 35;
    inline static const float BORDER_SIZE = 5;
    inline static const BarParam BAR_PARAM = {BAR_WIDTH, BAR_HEIGHT, BORDER_SIZE, 0xA811A6FF, 0xFFFFFFFF};
};