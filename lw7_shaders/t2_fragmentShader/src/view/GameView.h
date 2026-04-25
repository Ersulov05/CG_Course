#pragma once
#include "./Canvas/Canvas3D.h"
#include "./Meshes/Mesh.h"

class GameView {
public:
    GameView() {
    }

    void Run() {
        m_canvas.GetCamera().SetPosition(Point3D{0, 0, 1});
        m_canvas.Run(
        [this](ICanvas3D &canvas, float deltatime)
        {
            auto &camera = canvas.GetCamera();
            canvas.DrawMesh(m_flag);
        });
    }

private:
    Canvas3D m_canvas;
    static inline MeshData m_flag = Mesh::Flag(0xFFFFFFFF);
};