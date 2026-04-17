#pragma once
#include "./Canvas/Canvas3D.h"
#include "./ModelLoader/ModelLoader.h"
#include "../controller/GameController.h"
#include "./TankView.h"
#include "./MapView.h"
#include "./ShellView.h"

#include "./UI/UIView.h"

class GameView {
public:
    GameView(GameController& gameController): m_gameController(gameController) {
        auto & keyboardController = m_canvas.GetKeyboardController();

        keyboardController.OnKeyHoldSubscribe(this, GLFW_KEY_LEFT, [this](float deltatime){
            m_gameController.MoveTank(Direction::Left, deltatime);
        });
        keyboardController.OnKeyHoldSubscribe(this, GLFW_KEY_RIGHT, [this](float deltatime){
            m_gameController.MoveTank(Direction::Right, deltatime);
        });

        keyboardController.OnKeyHoldSubscribe(this, GLFW_KEY_UP, [this](float deltatime){
            m_gameController.MoveTank(Direction::Forward, deltatime);
        });

        keyboardController.OnKeyHoldSubscribe(this, GLFW_KEY_DOWN, [this](float deltatime){
            m_gameController.MoveTank(Direction::Backward, deltatime);
        });

        keyboardController.OnKeyPressedSubscribe(this, GLFW_KEY_SPACE, [this](){
            m_gameController.Fire();
        });
    }

    void Run() {
        m_canvas.SetLightPos({0, 5, 0});
        m_canvas.Run(
        [this](ICanvas3D &canvas, float deltatime)
        {
            auto &camera = canvas.GetCamera();
            camera.SetPosition(Point3D{0, 20, 24});
            camera.SetRotation(0, -40, 0);
            
            m_gameController.Update(deltatime);

            TankView::Draw(canvas, m_gameController.GetPlayerTank());
            MapView::Draw(canvas, m_gameController.GetMap());
            for (auto& shell : m_gameController.GetShells())
            {
               ShellView::Draw(canvas, shell); 
            }

            UIView::Draw(canvas, m_gameController);
        });
    }

private:
    Canvas3D m_canvas; 
    Vector3D m_rotation;
    GameController& m_gameController;
};