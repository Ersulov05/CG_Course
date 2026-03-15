#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <map>
#include <string>
#include <iostream>
#include <ft2build.h>
#include FT_FREETYPE_H

struct Character
{
    GLuint textureID;
    glm::ivec2 size;
    glm::ivec2 bearing;
    unsigned int advance;
};

class FontLoader
{
public:
    bool Initialize(const std::string &fontPath, unsigned int fontSize = 48)
    {
        m_fontSize = fontSize;
        FT_Library ft;
        if (FT_Init_FreeType(&ft))
        {
            std::cerr << "ERROR: Could not init FreeType" << std::endl;
            return false;
        }

        FT_Face face;
        if (FT_New_Face(ft, fontPath.c_str(), 0, &face))
        {
            std::cerr << "ERROR: Failed to load font: " << fontPath << std::endl;
            return false;
        }

        FT_Set_Pixel_Sizes(face, 0, fontSize);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        // Загружаем ASCII символы (32-128)
        for (unsigned char c = 32; c < 128; c++)
        {
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            {
                std::cerr << "ERROR: Failed to load Glyph: " << c << std::endl;
                continue;
            }

            GLuint texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            Character character = {
                texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                static_cast<unsigned int>(face->glyph->advance.x)};

            m_characters.insert(std::pair<char, Character>(c, character));
        }

        FT_Done_Face(face);
        FT_Done_FreeType(ft);

        return true;
    }

protected:
    std::map<char, Character> m_characters;
    unsigned int m_fontSize;
};