#pragma once
#include "./Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class UIShader : public Shader
{
public:
    UIShader(const glm::mat4 &t, const glm::mat4 &p, int &textureCount)
        : m_transform(t), m_projection(p), m_textureCount(textureCount) 
    {
        Load();
    }

    virtual void SetUniforms() override
    {
        glUniformMatrix4fv(GetUniform("uTransform"), 1, GL_FALSE, glm::value_ptr(m_transform));
        glUniformMatrix4fv(GetUniform("uProjection"), 1, GL_FALSE, glm::value_ptr(m_projection));
        glUniform1i(GetUniform("uTextureCount"), m_textureCount);

        // GLint projLoc = GetUniform("uProjection");
        // std::cout << "uProjection location: " << projLoc << std::endl;

        // if (projLoc != -1) {
        //     glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(m_projection));
        //     std::cout << "Projection matrix set:" << std::endl;
        //     std::cout << m_projection[0][0] << " " << m_projection[0][1] << " " << m_projection[0][2] << " " << m_projection[0][3] << std::endl;
        // } else {
        //     std::cout << "uProjection not found in shader!" << std::endl;
        // }



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
        return LoadFromFiles("ui.vert", "ui.frag");
    }
protected:
    const glm::mat4 &m_transform;
    const glm::mat4 &m_projection;
    const int &m_textureCount;
};
