#pragma once
#ifndef GLAD_GLAD_H
#include <glad/glad.h>
#endif
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "ShaderLoader.h"

class ShaderProgram
{
public:
    ShaderProgram() : m_programId(0) {}

    virtual ~ShaderProgram()
    {
        if (m_programId)
        {
            glDeleteProgram(m_programId);
        }
    }

    bool Load(const std::string &vertexPath, const std::string &fragmentPath)
    {
        m_programId = ShaderLoader::LoadShader(vertexPath, fragmentPath);
        return m_programId != 0;
    }

    void Use() const
    {
        glUseProgram(m_programId);
    }

    GLuint GetId() const
    {
        return m_programId;
    }

    GLint GetUniformLocation(const std::string &name)
    {
        auto it = m_uniformLocations.find(name);
        if (it != m_uniformLocations.end())
        {
            return it->second;
        }

        GLint location = glGetUniformLocation(m_programId, name.c_str());
        m_uniformLocations[name] = location;
        return location;
    }

    void SetUniformMatrix4fv(const std::string &name, const glm::mat4 &matrix)
    {
        glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
    }

    void SetUniformMatrix4fv(const std::string &name, const float *value)
    {
        glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, value);
    }

protected:
    GLuint m_programId;
    std::unordered_map<std::string, GLint> m_uniformLocations;
};