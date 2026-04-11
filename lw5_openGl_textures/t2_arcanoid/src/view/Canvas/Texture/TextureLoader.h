#pragma once

#include <glad/glad.h>
#include <string>
#include <unordered_map>
#include <vector>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

struct Texture {
    GLuint id;
    int width;
    int height;
    std::string path;
};

struct TextureFormat {
    GLenum internalFormat;
    GLenum format;
};

class TextureLoader {
public:
    ~TextureLoader() {
        cleanup();
    }

    Texture LoadTexture(const std::string& filepath) {
        auto it = m_textures.find(filepath);
        if (it != m_textures.end()) {
            return it->second;
        }
        
        int width, height, channels;
        stbi_set_flip_vertically_on_load(true);
        unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);
        
        if (!data) {
            std::cerr << "stbi_error: " << stbi_failure_reason() << std::endl;
            return Texture{0, 0, 0, filepath};
        }
        
        Texture texture = {0, width, height, filepath};
        glGenTextures(1, &texture.id);
        glBindTexture(GL_TEXTURE_2D, texture.id);
        auto textureFormat = GetTextureFormat(channels);
        glTexImage2D(GL_TEXTURE_2D, 0, textureFormat.internalFormat, width, height, 0, textureFormat.format, GL_UNSIGNED_BYTE, data);
        SetTextureParametri(GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR);        
        stbi_image_free(data);

        GLenum error = glGetError();
        if (error != GL_NO_ERROR) {
            std::cerr << "OpenGL error while loading texture " << filepath << ": " << error << std::endl;
            glDeleteTextures(1, &texture.id);
            return Texture{0, 0, 0, filepath};
        }
        
        m_textures[filepath] = texture;
        return texture;
    }
    
    Texture LoadTexture(
        const std::string& filepath, 
        GLint wrapU, GLint wrapV, 
        GLint minFilter, GLint magFilter
    ) {
        Texture texture = LoadTexture(filepath);
        bindTexture(texture);
        SetTextureParametri(wrapU, wrapV, minFilter, magFilter);
        
        return texture;
    }
    
    void bindTexture(const Texture& texture, GLuint unit = 0) {
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D, texture.id);
    }
    
    void unbindTextures() {
        for (int i = 0; i < 2; i++) {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }
    
    Texture getTexture(const std::string& filepath) {
        auto it = m_textures.find(filepath);
        if (it != m_textures.end()) {
            return it->second;
        }
        return Texture{0, 0, 0, ""};
    }
    
    void cleanup() {
        for (auto& pair : m_textures) {
            glDeleteTextures(1, &pair.second.id);
        }
        m_textures.clear();
    }
    
private:        
    std::unordered_map<std::string, Texture> m_textures;

    void SetTextureParametri(GLint wrapU, GLint wrapV, GLint minFilter, GLint magFilter)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapU);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapV);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
    }

    TextureFormat GetTextureFormat(int channels)
    {
        GLenum internalFormat;
        GLenum format;
        
        switch (channels) {
            case 1:
                internalFormat = GL_R8;
                format = GL_RED;
                break;
            case 2:
                internalFormat = GL_RG8;
                format = GL_RG;
                break;
            case 3:
                internalFormat = GL_RGB8;
                format = GL_RGB;
                break;
            case 4:
                internalFormat = GL_RGBA8;
                format = GL_RGBA;
                break;
            default:
                internalFormat = GL_RGB8;
                format = GL_RGB;
                break;
        }

        return TextureFormat{internalFormat, format};
    }
};