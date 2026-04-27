#pragma once
#include "./Shader.h"
#include <glm/glm.hpp>

class BaseShader : public Shader
{
public:
    BaseShader(const glm::mat4 &t, const glm::mat4 &p, const glm::mat4 &v)
        : m_transform(t), m_projection(p), m_view(v) {}

    virtual void SetUniforms() override
    {
        glUniformMatrix4fv(GetUniform("uTransform"), 1, GL_FALSE, glm::value_ptr(m_transform));
        glUniformMatrix4fv(GetUniform("uProjection"), 1, GL_FALSE, glm::value_ptr(m_projection));
        glUniformMatrix4fv(GetUniform("uView"), 1, GL_FALSE, glm::value_ptr(m_view));
    }

protected:
    const glm::mat4 &m_transform;
    const glm::mat4 &m_projection;
    const glm::mat4 &m_view;
};
