#pragma once
#include "../Canvas/ICanvas3D.h"
#include "../ModelLoader/ModelLoader.h"
#include "../../model/Map/Headquarters/Headquarters.h"
#include "../Meshes/Mesh.h"
#include "../../model/Constants.h"
#include "../ObjectHealthView.h"
#include "../Canvas/FpvCamera3D.h"

class HeadquartersView {
public:
    static void Draw(ICanvas3D& canvas, const Headquarters& headquarters, const FpvCamera3D& camera) 
    {
        auto size = headquarters.GetSize();
        canvas.GetTransform().PushMatrix();
        canvas.GetTransform().Translate(headquarters.GetPosition());
        canvas.GetTransform().Translate({0, size.height/2, 0});
        canvas.GetTransform().Scale(size);

        canvas.DrawMesh(m_headquartersModel);

        canvas.GetTransform().PopMatrix();

        DrawHealth(canvas, headquarters, camera);
    }
private:
    inline static const MeshData m_headquartersModel = ModelLoader::LoadModel("./models/base/base.obj");
    inline static const float HEALTH_Y_OFFSET = 2;

    static void DrawHealth(ICanvas3D& canvas, const Headquarters& headquarters, const FpvCamera3D& camera)
    {
        auto healthPos = headquarters.GetPosition();
        healthPos.y += HEALTH_Y_OFFSET + headquarters.GetSize().height/2;
        BarData barData = {headquarters.GetHealth(), headquarters.GetTotalHealth()};
        ObjectHealthView::Draw(canvas, healthPos, camera, barData);
    }
};