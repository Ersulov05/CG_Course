#pragma once
#include "../model/Ball.h"
#include "./Canvas/ICanvas3D.h"
#include "./Meshes/Mesh.h"

class BallView {
public:
    static void Draw(const Ball& ball, ICanvas3D &canvas) {
        canvas.GetTransform().PushMatrix();
        canvas.GetTransform().Translate(ball.GetPosition());
        canvas.GetTransform().Translate(0, ball.GetRadius(), 0);
        canvas.GetTransform().Scale(ball.GetRadius() * 2);
        
        canvas.DrawMesh(m_ballMesh);
        canvas.GetTransform().PopMatrix();
    }

private:
    inline static const MeshData m_ballMesh = Mesh::Sphere(0xFF0000FF);
};