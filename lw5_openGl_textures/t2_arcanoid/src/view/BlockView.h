#pragma once
#include "../model/Block.h"
#include "./Canvas/ICanvas3D.h"
#include "./Meshes/Mesh.h"

class BlockView {
public:
    static void Draw(const Block& block, ICanvas3D &canvas) {
        canvas.GetTransform().PushMatrix();
        canvas.GetTransform().Translate(block.GetPosition());
        canvas.GetTransform().Translate(0, block.GetSize().height/2, 0);
        canvas.GetTransform().Scale(block.GetSize());
        
        canvas.DrawMesh(m_blockMesh);
        canvas.GetTransform().PopMatrix();
    }

private:
    inline static const MeshData m_blockMesh = Mesh::Cube(0x00FF00FF);
};