#pragma once
#include "./BaseShader.h"
#include <glm/glm.hpp>

class BasicShader : public BaseShader
{
public:
    BasicShader(const glm::mat4 &transform,
                const glm::mat4 &projection,
                const glm::mat4 &view)
        : BaseShader(transform, projection, view)
    {
        Load();
    }

    bool Load() override
    {
        return LoadFromFiles("basic.vert", "basic.frag");
    }
};
