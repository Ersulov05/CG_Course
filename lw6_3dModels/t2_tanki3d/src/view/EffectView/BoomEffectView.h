#pragma once
#include "../Canvas/ICanvas3D.h"
#include "../Meshes/Mesh.h"
#include "../Canvas/Shader/BoomShader.h"
#include "../../model/Effect/Effect.h"

class BoomEffectView {
public:
    static void Draw(ICanvas3D& canvas, const std::shared_ptr<IEffect>& effect)
    {           
        auto& shaderManager = canvas.GetShaderManager();
        auto* boomShader = dynamic_cast<BoomShader*>(shaderManager.GetByName("boom"));
        if (!boomShader) return;

        boomShader->SetTime(1 - effect->GetLifetime() / effect->GetTotalLifetime());
        boomShader->SetSize(0.5);

        shaderManager.Push(boomShader);
        canvas.GetTransform().PushMatrix();
        canvas.GetTransform().Translate(effect->GetPosition());

        canvas.DrawMesh(m_boomModel);

        canvas.GetTransform().PopMatrix();
        shaderManager.Pop();
    }

private:
    inline static const MeshData m_boomModel = Mesh::Sphere(0xFFFFFFFF, {}, 0.5, 8, 8);
};