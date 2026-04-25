#pragma once
#include "./BaseShader.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class SinShader : public BaseShader
{
public:
    SinShader(const glm::mat4 &transform,
                const glm::mat4 &projection,
                const glm::mat4 &view,
                const glm::vec3 &lightPos
            )
        : BaseShader(transform, projection, view), m_lightPos(lightPos)
    {
        Load();
    }

    virtual void SetUniforms() override
    {
        BaseShader::SetUniforms();
        glUniform3fv(GetUniform("uLightPos"), 1, glm::value_ptr(m_lightPos));
    }

    bool Load() override
    {
        return LoadFromFiles("sin.vert", "sin.frag");
    }
private:
    const glm::vec3 &m_lightPos;
};
