#pragma once
#include "./BaseShader.h"
#include <glm/glm.hpp>

class BasicShader : public BaseShader
{
public:
    BasicShader(const glm::mat4 &transform,
                const glm::mat4 &projection,
                const glm::mat4 &view,
                const glm::vec3 &lightPos,
                int &textureCount
            )
        : BaseShader(transform, projection, view), m_lightPos(lightPos), m_textureCount(textureCount)
    {
        Load();
    }

    virtual void SetUniforms() override
    {
        BaseShader::SetUniforms();
        glUniform3fv(GetUniform("uLightPos"), 1, glm::value_ptr(m_lightPos));
        glUniform1i(GetUniform("uTextureCount"), m_textureCount);

        for (int i = 0; i < m_textureCount && i < 2; i++) {
            std::string uniformName = "uTextures[" + std::to_string(i) + "]";
            GLint loc = GetUniform(uniformName);
            if (loc != -1) {
                glUniform1i(loc, i);
            }
        }
    }

    bool Load() override
    {
        return LoadFromFiles("basic.vert", "basic.frag");
    }
private:
    const glm::vec3 &m_lightPos;
    const int &m_textureCount;
};
