#pragma once
#include "./Canvas/ICanvas3D.h"
#include "./ModelLoader/ModelLoader.h"
#include "../model/Shell/Shell.h"
#include "./Meshes/Mesh.h"

class ShellView {
public:
    static void Draw(ICanvas3D& canvas, const Shell& shell) 
    {
        canvas.GetTransform().PushMatrix();
        canvas.GetTransform().Translate(shell.GetPosition());
        canvas.GetTransform().Rotate(shell.GetRotation());
        canvas.GetTransform().Scale(shell.GetSize());

        canvas.DrawMesh(m_shellModel);

        canvas.GetTransform().PopMatrix();
    }
private:
    inline static const MeshData m_shellModel = Mesh::Cube(0xFFFFFFFF, {"./textures/bonus.jpg"}, {0.2, 0.2, 0.4});
};