#pragma once
#include "./BaseShader.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class ArrowShader : public BaseShader
{
public:
    ArrowShader(const glm::mat4 &transform,
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
        return LoadFromFiles("arrow.vert", "arrow.frag", "arrow.geom");
    }
private:
};
