#pragma once
#include "./Canvas/Canvas3D.h"
#include "./ModelLoader/ModelLoader.h"
#include "./MapView.h"
#include "./CarView.h"
#include "../model/Game.h"

class GameView {
public:
    GameView(Game& game): m_game(game) {
        auto & keyboardController = m_canvas.GetKeyboardController();

        keyboardController.OnKeyHoldSubscribe(this, GLFW_KEY_LEFT, [this](float deltatime){
            m_rotation.y -= 120 * deltatime;
        });
        keyboardController.OnKeyHoldSubscribe(this, GLFW_KEY_RIGHT, [this](float deltatime){
            m_rotation.y += 120 * deltatime;
        });
    }

    void Run() {
        m_canvas.SetLightPos({0, 5, 0});
        m_canvas.Run(
        [this](ICanvas3D &canvas, float deltatime)
        {
            auto &camera = canvas.GetCamera();
            camera.SetPosition(Point3D{0, 8, 12});
            camera.SetRotation(0, -40, 0);
            m_game.Update(deltatime);
            canvas.GetTransform().PushMatrix();
            canvas.GetTransform().RotateY(m_rotation.y);
            MapView::Draw(canvas);
            CarView::Draw(canvas, m_game.GetPoliceCar(), true);
            CarView::Draw(canvas, m_game.GetCar(), false);
            canvas.GetTransform().PopMatrix();
        });
    }

private:
    Canvas3D m_canvas; 
    Vector3D m_rotation;
    Game m_game;
};