#pragma once
#include "./BaseShader.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class BasicShader : public BaseShader
{
public:
    BasicShader(const glm::mat4 &transform,
                const glm::mat4 &projection,
                int &textureCount
            )
        : BaseShader(transform, projection), m_textureCount(textureCount), m_time(0)
    {
        Load();
    }

    virtual void SetUniforms() override
    {
        BaseShader::SetUniforms();
        glUniform1i(GetUniform("uTextureCount"), m_textureCount);
        glUniform1f(GetUniform("uTime"), m_time);
        glUniform2fv(GetUniform("uClickUV"), 1, glm::value_ptr(m_clickPos));

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

    void SetTime(float time)
    {
        m_time = time;
    }

    void SetClickPos(glm::vec2 clickPos)
    {
        m_clickPos = clickPos;
    }
private:
    const int &m_textureCount;
    glm::vec2 m_clickPos;
    float m_time;
};
