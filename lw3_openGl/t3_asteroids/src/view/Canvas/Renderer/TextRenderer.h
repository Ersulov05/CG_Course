// TextRenderer.h
#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>
#include <string>
#include <iostream>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "../../../common/Point.h"
#include "../../../common/Color.h"
#include "../../../common/Size.h"

#include "./FontLoader.h"

class TextRenderer : public FontLoader
{
public:
    TextRenderer(){};

    ~TextRenderer()
    {
        for (auto &[c, ch] : m_characters)
        {
            glDeleteTextures(1, &ch.textureID);
        }

        if (m_VAO)
            glDeleteVertexArrays(1, &m_VAO);
        if (m_VBO)
            glDeleteBuffers(1, &m_VBO);
    }

    bool Initialize(const std::string &fontPath, unsigned int fontSize = 48)
    {
        SetupBuffers();
        return FontLoader::Initialize(fontPath, fontSize);
    }

    void RenderText(const std::string &text, float x, float y, float size, const Color &color, GLuint shaderProgram)
    {
        if (m_characters.empty())
            return;

        SaveOpenGLState();
        SetupShaderUniforms(shaderProgram, color);

        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(m_VAO);

        DrawTextCharacters(text, x, y, size, color);

        DisableTexture(shaderProgram);
        RestoreOpenGLState();
    }

    Size GetTextBoundSize(const std::string &text, float size)
    {
        const float scale = size / m_fontSize;
        Size boundSize;
        for (char c : text)
        {
            auto it = m_characters.find(c);
            if (it != m_characters.end())
            {
                boundSize.width += (it->second.advance >> 6) * scale;
                float height = it->second.size.y * scale;
                if (height > boundSize.height)
                    boundSize.height = height;
            }
        }
        return boundSize;
    }

private:
    const int COUNT_SUMBOL_VERTICES = 6;
    const int VERTEX_COMPONENTS_COUNT = 8;
    GLuint m_VAO = 0;
    GLuint m_VBO = 0;
    GLint m_previousVAO = 0;
    GLint m_previousVBO = 0;
    GLint m_previousTexture = 0;

    void DrawTextCharacters(const std::string &text, float x, float y, float size, const Color &color)
    {
        const float scale = size / m_fontSize;
        float originalX = x;
        for (char c : text)
        {
            if (c == '\n')
            {
                x = originalX;
                y += size;
                continue;
            }

            auto it = m_characters.find(c);
            if (it == m_characters.end())
                continue;

            Character ch = it->second;
            auto vertices = GetCharVertices(x, y, scale, color, ch);

            glBindTexture(GL_TEXTURE_2D, ch.textureID);
            glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(float), vertices.data());
            glDrawArrays(GL_TRIANGLES, 0, 6);

            x += (ch.advance >> 6) * scale;
        }
    }

    void SaveOpenGLState()
    {
        glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &m_previousVAO);
        glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &m_previousVBO);
        glGetIntegerv(GL_TEXTURE_BINDING_2D, &m_previousTexture);
    }

    void RestoreOpenGLState()
    {
        glBindVertexArray(m_previousVAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_previousVBO);
        glBindTexture(GL_TEXTURE_2D, m_previousTexture);
    }

    void SetupShaderUniforms(GLuint shaderProgram, const Color &color)
    {
        SetTextColor(shaderProgram, color);
        EnableTexture(shaderProgram);
    }

    void SetTextColor(GLuint shaderProgram, const Color &color) const
    {
        GLint colorLoc = glGetUniformLocation(shaderProgram, "textColor");
        if (colorLoc != -1)
        {
            glUniform3f(colorLoc, color.r, color.g, color.b);
        }
    }

    void EnableTexture(GLuint shaderProgram) const
    {
        GLint useTexLoc = glGetUniformLocation(shaderProgram, "uUseTexture");
        if (useTexLoc != -1)
        {
            glUniform1i(useTexLoc, 1);
        }

        GLint texLoc = glGetUniformLocation(shaderProgram, "uTexture");
        if (texLoc != -1)
        {
            glUniform1i(texLoc, 0);
        }
    }

    void DisableTexture(GLuint shaderProgram) const
    {
        GLint useTexLoc = glGetUniformLocation(shaderProgram, "uUseTexture");
        if (useTexLoc != -1)
        {
            glUniform1i(useTexLoc, 0);
        }
    }

    std::vector<float> GetCharVertices(float x, float y, float scale, const Color &color, const Character &ch)
    {
        std::vector<float> vertices;

        vertices.reserve(6 * 8);

        float xpos = x + ch.bearing.x * scale;
        float ypos = y + (ch.size.y - ch.bearing.y) * scale;
        float w = ch.size.x * scale;
        float h = ch.size.y * scale;

        vertices.insert(vertices.end(), {xpos, ypos - h, color.r, color.g, color.b, color.a, 0.0f, 0.0f});
        vertices.insert(vertices.end(), {xpos, ypos, color.r, color.g, color.b, color.a, 0.0f, 1.0f});
        vertices.insert(vertices.end(), {xpos + w, ypos, color.r, color.g, color.b, color.a, 1.0f, 1.0f});

        vertices.insert(vertices.end(), {xpos, ypos - h, color.r, color.g, color.b, color.a, 0.0f, 0.0f});
        vertices.insert(vertices.end(), {xpos + w, ypos, color.r, color.g, color.b, color.a, 1.0f, 1.0f});
        vertices.insert(vertices.end(), {xpos + w, ypos - h, color.r, color.g, color.b, color.a, 1.0f, 0.0f});

        return vertices;
    }

    void SetupBuffers()
    {
        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);

        glBindVertexArray(m_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * COUNT_SUMBOL_VERTICES * VERTEX_COMPONENTS_COUNT, NULL, GL_DYNAMIC_DRAW);

        // Атрибут 0: позиция (vec2)
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, VERTEX_COMPONENTS_COUNT * sizeof(float), (void *)0);

        // Атрибут 1: цвет (vec4)
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, VERTEX_COMPONENTS_COUNT * sizeof(float), (void *)(2 * sizeof(float)));

        // Атрибут 2: текстурные координаты (vec2)
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, VERTEX_COMPONENTS_COUNT * sizeof(float), (void *)(6 * sizeof(float)));

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
};
