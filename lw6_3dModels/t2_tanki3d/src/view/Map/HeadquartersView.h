#pragma once
#include "../Canvas/ICanvas3D.h"
#include "../ModelLoader/ModelLoader.h"
#include "../../model/Map/Headquarters/Headquarters.h"
#include "../Meshes/Mesh.h"
#include "../../model/Constants.h"

class HeadquartersView {
public:
    static void Draw(ICanvas3D& canvas, const Headquarters& headquarters) 
    {
        canvas.GetTransform().PushMatrix();
        canvas.GetTransform().Translate(headquarters.GetPosition());
        canvas.GetTransform().Scale(headquarters.GetSize());

        canvas.DrawMesh(m_headquartersModel);

        canvas.GetTransform().PopMatrix();
    }
private:
    inline static const MeshData m_headquartersModel = Mesh::Cube(0xFFFFFFFF);
};