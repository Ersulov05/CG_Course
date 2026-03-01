#pragma once
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

class ShaderLoader
{
public:
    static GLuint LoadShader(const std::string &vertexPath, const std::string &fragmentPath)
    {
        std::string vertexCode = ReadFile(vertexPath);
        std::string fragmentCode = ReadFile(fragmentPath);

        if (vertexCode.empty() || fragmentCode.empty())
        {
            return 0;
        }

        GLuint vertexShader = CompileShader(GL_VERTEX_SHADER, vertexCode);
        GLuint fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentCode);

        GLuint program = glCreateProgram();
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);
        glLinkProgram(program);

        CheckLinkErrors(program);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        return program;
    }

private:
    static std::string ReadFile(const std::string &filepath)
    {
        std::ifstream file(filepath);
        if (!file.is_open())
        {
            std::cerr << "Failed to open file: " << filepath << std::endl;
            return "";
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

    static GLuint CompileShader(GLenum type, const std::string &source)
    {
        GLuint shader = glCreateShader(type);
        const char *src = source.c_str();
        glShaderSource(shader, 1, &src, nullptr);
        glCompileShader(shader);

        CheckCompileErrors(shader, type);
        return shader;
    }

    static void CheckCompileErrors(GLuint shader, GLenum type)
    {
        int success;
        char infoLog[512];
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

        if (!success)
        {
            glGetShaderInfoLog(shader, 512, nullptr, infoLog);
            std::string typeName = (type == GL_VERTEX_SHADER) ? "VERTEX" : "FRAGMENT";
            std::cerr << "Shader compilation error (" << typeName << "):\n"
                      << infoLog << std::endl;
        }
    }

    static void CheckLinkErrors(GLuint program)
    {
        int success;
        char infoLog[512];
        glGetProgramiv(program, GL_LINK_STATUS, &success);

        if (!success)
        {
            glGetProgramInfoLog(program, 512, nullptr, infoLog);
            std::cerr << "Shader program linking error:\n"
                      << infoLog << std::endl;
        }
    }
};
