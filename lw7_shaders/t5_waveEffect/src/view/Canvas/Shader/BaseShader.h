#pragma once
#include "./Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class BaseShader : public Shader
{
public:
    BaseShader(const glm::mat4 &t, const glm::mat4 &p)
        : m_transform(t), m_projection(p) {}

    virtual void SetUniforms() override
    {
        glUniformMatrix4fv(GetUniform("uTransform"), 1, GL_FALSE, glm::value_ptr(m_transform));
        glUniformMatrix4fv(GetUniform("uProjection"), 1, GL_FALSE, glm::value_ptr(m_projection));
    }

protected:
    const glm::mat4 &m_transform;
    const glm::mat4 &m_projection;
};
