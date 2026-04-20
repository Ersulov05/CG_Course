#pragma once
#include "./Canvas/Canvas3D.h"
#include "./ModelLoader/ModelLoader.h"
#include "../controller/GameController.h"
#include "./TankView/TankView.h"
#include "./MapView/MapView.h"
#include "./ShellView.h"
#include "./UI/UIView.h"
#include "./BonusView/BonusView.h"

#include "./EffectView/EffectView.h"

class GameView {
public:
    GameView(GameController& gameController): m_gameController(gameController) {
        SetSubscribeKeyboard();
    }

    void Run() {
        m_canvas.SetLightPos({0, 50, 0});
        m_canvas.Run(
        [this](ICanvas3D &canvas, float deltatime)
        {
            auto &camera = canvas.GetCamera();
            camera.SetPosition(Point3D{0, 35, 30});
            camera.SetRotation(0, -55, 0);

            // camera.SetPosition(Point3D{0, 10, 10});
            // camera.SetRotation(0, -30, 0);

            UpdateMoveTank();
            m_gameController.Update(deltatime);

            TankView::Draw(canvas, m_gameController.GetPlayerTank(), camera);
            MapView::Draw(canvas, m_gameController.GetMap(), camera);
            DrawEnemies(canvas, camera);
            DrawShells(canvas);
            DrawBonuses(canvas);
            DrawEffects(canvas);

            UIView::Draw(canvas, m_gameController);

        });
    }

private:
    Canvas3D m_canvas; 
    Vector3D m_rotation;
    GameController& m_gameController;
    std::vector<Direction> m_pressedDirections;


    void UpdateMoveTank() 
    {
        if (!m_pressedDirections.empty()) {
            m_gameController.RotateTank(m_pressedDirections.back());
            m_gameController.MoveTank();
        } else {
            m_gameController.StopTank();
        }
    }

    void DrawShells(ICanvas3D& canvas)
    {
        for (auto& shell : m_gameController.GetShells())
        {
            ShellView::Draw(canvas, shell); 
        }
    }

    void DrawEnemies(ICanvas3D& canvas, const FpvCamera3D& camera)
    {
        for (auto& tank : m_gameController.GetEnemyTanks())
        {
            TankView::Draw(canvas, tank, camera, true);
        }
    }

    void DrawBonuses(ICanvas3D& canvas)
    {
        for (auto& bonus : m_gameController.GetBonuses())
        {
            BonusView::Draw(canvas, bonus);
        }
    }

    void DrawEffects(ICanvas3D& canvas)
    {
        for (auto& effect : m_gameController.GetEffects())
        {
            EffectView::Draw(canvas, effect);
        }
    }

    void SetSubscribeKeyboard()
    {
        auto & keyboardController = m_canvas.GetKeyboardController();

        keyboardController.OnKeyPressedSubscribe(this, GLFW_KEY_LEFT, [this](){
            AddPressedDirection(Direction::Left);
        });
        keyboardController.OnKeyPressedSubscribe(this, GLFW_KEY_RIGHT, [this](){
            AddPressedDirection(Direction::Right);
        });
        keyboardController.OnKeyPressedSubscribe(this, GLFW_KEY_UP, [this](){
            AddPressedDirection(Direction::Forward);
        });
        keyboardController.OnKeyPressedSubscribe(this, GLFW_KEY_DOWN, [this](){
            AddPressedDirection(Direction::Backward);
        });

        keyboardController.OnKeyReleasedSubscribe(this, GLFW_KEY_LEFT, [this](){
            RemovePressedDirection(Direction::Left);
        });
        keyboardController.OnKeyReleasedSubscribe(this, GLFW_KEY_RIGHT, [this](){
            RemovePressedDirection(Direction::Right);
        });
        keyboardController.OnKeyReleasedSubscribe(this, GLFW_KEY_UP, [this](){
            RemovePressedDirection(Direction::Forward);
        });
        keyboardController.OnKeyReleasedSubscribe(this, GLFW_KEY_DOWN, [this](){
            RemovePressedDirection(Direction::Backward);
        });

        keyboardController.OnKeyPressedSubscribe(this, GLFW_KEY_SPACE, [this](){
            m_gameController.Fire();
        });
    }

    void AddPressedDirection(Direction dir) {
        RemovePressedDirection(dir);
        m_pressedDirections.push_back(dir);
    }
    
    void RemovePressedDirection(Direction dir) {
        auto it = std::find(m_pressedDirections.begin(), m_pressedDirections.end(), dir);
        if (it != m_pressedDirections.end()) {
            m_pressedDirections.erase(it);
        }
    }
};