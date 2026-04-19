#pragma once
#include "./BaseShader.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class BoomShader : public BaseShader
{
public:
    BoomShader(const glm::mat4 &transform,
                const glm::mat4 &projection,
                const glm::mat4 &view
            )
        : BaseShader(transform, projection, view), m_time(0), m_size(1)
    {
        Load();
    }

    virtual void SetUniforms() override
    {
        BaseShader::SetUniforms();
        glUniform1f(GetUniform("uTime"), m_time);
        glUniform1f(GetUniform("uScale"), m_size);
    }

    bool Load() override
    {
        return LoadFromFiles("boom.vert", "boom.frag");
    }

    void SetTime(float time)
    {
        m_time = time;
    }

    void SetSize(float size)
    {
        m_size = size;
    }
private:
    float m_time;
    float m_size;
};
