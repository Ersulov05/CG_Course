#pragma once
#include <unordered_map>
#include <glad/glad.h>
#include <string>
#include "./ShaderLoader.h"

const std::string SHADERS_DIR = "./shaders/";

class Shader
{
public:
    virtual ~Shader() = default;

    virtual void SetUniforms() = 0;

    virtual bool Load() = 0;

    virtual void Use()
    {
        glUseProgram(m_programId);
        SetUniforms();
    }

    bool LoadFromFiles(const std::string &vertexFile, const std::string &fragFile)
    {
        std::string vertexPath = SHADERS_DIR + vertexFile;
        std::string fragPath = SHADERS_DIR + fragFile;

        m_programId = ShaderLoader::LoadShader(vertexPath, fragPath);
        return m_programId != 0;
    }

    GLint GetUniform(const std::string &name)
    {
        auto it = m_uniforms.find(name);
        if (it != m_uniforms.end())
            return it->second;

        GLint loc = glGetUniformLocation(m_programId, name.c_str());
        m_uniforms[name] = loc;
        return loc;
    }

protected:
    GLuint m_programId;
    std::unordered_map<std::string, GLint> m_uniforms;
};
