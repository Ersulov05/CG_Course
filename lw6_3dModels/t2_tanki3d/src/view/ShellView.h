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
        canvas.GetTransform().Scale(shell.GetSize() * 2);

        canvas.DrawMesh(m_shellModel);

        canvas.GetTransform().PopMatrix();
    }
private:
    inline static const MeshData m_shellModel = Mesh::Sphere(0x2F2F2FFF, {"./textures/metal2.jpg"});
};