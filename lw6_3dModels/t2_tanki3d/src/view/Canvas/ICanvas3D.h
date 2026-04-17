#pragma once
#include <functional>
#include "./RendererData.h"
#include "./Transform.h"
#include "./FpvCamera3D.h"
#include "./Controller/KeyboardController.h"
#include "./Shader/ShaderManager.h"

class ICanvas3D
{
public:
    using UpdateCallback = std::function<void(ICanvas3D &, float deltaTime)>;
    virtual ~ICanvas3D() = default;

    virtual void DrawMesh(const MeshData &mesh) = 0;
    virtual void Run(UpdateCallback updateCallback) = 0;

    virtual unsigned GetWidth() const = 0;
    virtual unsigned GetHeight() const = 0;
    virtual Transform &GetTransform() = 0;
    virtual KeyboardController &GetKeyboardController() = 0;
    virtual FpvCamera3D &GetCamera() = 0;
    virtual ShaderManager &GetShaderManager() = 0;
};