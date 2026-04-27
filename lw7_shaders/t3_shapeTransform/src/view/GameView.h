#pragma once
#include "./Canvas/Canvas3D.h"
#include "./MorphingSurface.h"
#include "./Meshes/Mesh.h"

class GameView {
public:
    GameView() {
        auto& keyboardController = m_canvas.GetKeyboardController();
        keyboardController.OnKeyHoldSubscribe(this, GLFW_KEY_RIGHT, [this](float deltatime){
            m_rotation.RotateY(1 * deltatime);
        });
        keyboardController.OnKeyHoldSubscribe(this, GLFW_KEY_LEFT, [this](float deltatime){
            m_rotation.RotateY(-1 * deltatime);
        });
        keyboardController.OnKeyHoldSubscribe(this, GLFW_KEY_UP, [this](float deltatime){
            m_rotation.RotateX(1 * deltatime);
        });
        keyboardController.OnKeyHoldSubscribe(this, GLFW_KEY_DOWN, [this](float deltatime){
            m_rotation.RotateX(-1 * deltatime);
        });
    }

    void Run() {
        m_canvas.GetCamera().SetPosition(Point3D{0, 5, 5});
        m_canvas.GetCamera().SetRotation(Vector3D{0, -45, 0});
        m_canvas.SetLightPos(Point3D{0, 3, 0});
        MorphingSurface ms(32);
        m_canvas.Run(
        [this, &ms](ICanvas3D &canvas, float deltatime)
        {
            auto &camera = canvas.GetCamera();
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
            ms.Draw(canvas, m_time);
            canvas.GetTransform().PopMatrix();
        });
    }

private:
    Canvas3D m_canvas;
    Quaternion3D m_rotation;
    float m_time;
    float m_addCoef = 0.5;
    inline static const MeshData m_mesh = Mesh::Cube(0xFFFFFFFF);
};