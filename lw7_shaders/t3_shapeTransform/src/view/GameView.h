#pragma once
#include "./Canvas/Canvas3D.h"
#include "./Surface.h"

class GameView {
public:
    GameView() {
        auto& keyboardController = m_canvas.GetKeyboardController();
        keyboardController.OnKeyHoldSubscribe(this, GLFW_KEY_RIGHT, [this](float deltatime){
            m_rotation.RotateY(2 * deltatime);
        });
        keyboardController.OnKeyHoldSubscribe(this, GLFW_KEY_LEFT, [this](float deltatime){
            m_rotation.RotateY(-2 * deltatime);
        });
        keyboardController.OnKeyHoldSubscribe(this, GLFW_KEY_UP, [this](float deltatime){
            m_rotation.RotateX(2 * deltatime);
        });
        keyboardController.OnKeyHoldSubscribe(this, GLFW_KEY_DOWN, [this](float deltatime){
            m_rotation.RotateX(-2 * deltatime);
        });
    }

    void Run() {
        m_canvas.GetCamera().SetPosition(Point3D{0, 5, 5});
        m_canvas.GetCamera().SetRotation(Vector3D{0, -45, 0});
        m_canvas.SetLightPos(Point3D{0, 5, 0});
        Surface surface;
        m_canvas.Run(
        [this, &surface](ICanvas3D &canvas, float deltatime)
        {
            canvas.GetTransform().PushMatrix();

            m_time += m_addCoef * deltatime;

            if (m_time >= 1) {
                m_time = 1;
                m_addCoef *= -1;
            }
            if (m_time <= 0) {
                m_time = 0;
                m_addCoef *= -1;
            }

            canvas.GetTransform().Rotate(m_rotation);
            surface.Draw(canvas, m_time);
            canvas.GetTransform().PopMatrix();
        });
    }

private:
    Canvas3D m_canvas;
    Quaternion3D m_rotation;
    float m_time;
    float m_addCoef = 0.5;
};