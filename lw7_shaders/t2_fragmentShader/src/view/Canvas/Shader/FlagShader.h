#pragma once
#include "./BaseShader.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class FlagShader : public BaseShader
{
public:
    FlagShader(
        const glm::mat4 &transform,
        const glm::mat4 &projection,
        const glm::mat4 &view
    )
        : BaseShader(transform, projection, view)
    {
        Load();
    }

    virtual void SetUniforms() override
    {
        BaseShader::SetUniforms();
    }

    bool Load() override
    {
        return LoadFromFiles("flag.vert", "flag.frag");
    }
};
