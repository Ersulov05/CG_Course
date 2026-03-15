#pragma once
#include "./Shader.h"
#include <glm/glm.hpp>

class BaseShader : public Shader
{
public:
    BaseShader(const glm::mat4 &t, const glm::mat4 &p, const glm::mat4 &v)
        : transform(t), projection(p), view(v) {}

    virtual void SetUniforms() override
    {
        glUniformMatrix4fv(GetUniform("uTransform"), 1, GL_FALSE, glm::value_ptr(transform));
        glUniformMatrix4fv(GetUniform("uProjection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(GetUniform("uView"), 1, GL_FALSE, glm::value_ptr(view));
    }

protected:
    const glm::mat4 &transform;
    const glm::mat4 &projection;
    const glm::mat4 &view;
};
