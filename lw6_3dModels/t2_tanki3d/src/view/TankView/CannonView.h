#pragma once
#include "../Canvas/ICanvas3D.h"
#include "../ModelLoader/ModelLoader.h"
#include "../../model/Tank/Cannon/Cannon.h"

class CannonView {
public:
    static void Draw(ICanvas3D& canvas, const Cannon& cannon) 
    {
        canvas.GetTransform().PushMatrix();
        canvas.GetTransform().Translate(cannon.GetLocalPosition());

        auto cannonMesh = GetCannonMeshByType(cannon.GetCannonType());
        if (cannon.GetMountType() == MountType::TWIN) {
            auto offset = cannon.GetCannonOffset();
            canvas.GetTransform().Translate({offset, 0, 0});
            canvas.GetTransform().PushMatrix();
            canvas.GetTransform().Scale(m_cannonSize);
            canvas.DrawMesh(cannonMesh);
            canvas.GetTransform().PopMatrix();
            canvas.GetTransform().Translate({-offset * 2, 0, 0});
            canvas.GetTransform().PushMatrix();
            canvas.GetTransform().Scale(m_cannonSize);
            canvas.DrawMesh(cannonMesh);
            canvas.GetTransform().PopMatrix();
        } else {
            canvas.GetTransform().PushMatrix();
            canvas.GetTransform().Scale(m_cannonSize);
            canvas.DrawMesh(cannonMesh);
            canvas.GetTransform().PopMatrix();
        }

        canvas.GetTransform().PopMatrix();
    }
private:
    inline static const MeshData m_cannon85mmModel = ModelLoader::LoadModel("./models/cannon/cannon85mm.obj");
    inline static const MeshData m_cannon152mmModel = ModelLoader::LoadModel("./models/cannon/cannon152mm.obj");
    inline static const MeshData m_cannon100mmModel = ModelLoader::LoadModel("./models/cannon/cannon100mm.obj");
    inline static const Size3D m_cannonSize = {1.1, 1.1, 1};

    static MeshData GetCannonMeshByType(CannonType type) 
    {
        switch (type) {
            case CannonType::MM85:
                return m_cannon85mmModel;
            case CannonType::MM152:
                return m_cannon152mmModel;
            case CannonType::MM100:
                return m_cannon100mmModel;
            default:
                return m_cannon85mmModel;
        }
    }
};