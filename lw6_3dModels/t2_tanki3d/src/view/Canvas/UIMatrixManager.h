#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

class UIMatrixManager
{
public:
    UIMatrixManager(int screenWidth = 800, int screenHeight = 600)
        : m_screenWidth(screenWidth), m_screenHeight(screenHeight)
    {
        UpdateMatrix();
    }

    void SetScreenSize(int width, int height)
    {
        m_screenWidth = width;
        m_screenHeight = height;
        UpdateMatrix();
    }

    // Пиксельные координаты (0,0) в левом верхнем углу
    const glm::mat4& GetPixelMatrix() const { return m_pixelMatrix; }
    
    glm::mat4 GetNormalizedMatrix() const { return glm::mat4(1.0f); }
    
    glm::mat4 GetWorldMatrix(float worldSize) const
    {
        float aspect = (float)m_screenWidth / (float)m_screenHeight;
        return glm::ortho(
            -worldSize * aspect, worldSize * aspect,
            -worldSize, worldSize,
            -1.0f, 1.0f
        );
    }
    
    glm::mat4 GetCustomMatrix(float left, float right, float bottom, float top) const
    {
        return glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
    }
    
    glm::mat4 GetCenteredMatrix(float size = 200.0f) const
    {
        float aspect = (float)m_screenWidth / (float)m_screenHeight;
        return glm::ortho(
            -size * aspect / 2.0f, size * aspect / 2.0f,
            -size / 2.0f, size / 2.0f,
            -1.0f, 1.0f
        );
    }

private:
    void UpdateMatrix()
    {
        m_pixelMatrix = glm::ortho(
            0.0f, (float)m_screenWidth,
            (float)m_screenHeight, 0.0f,
            -1.0f, 1.0f
        );
    }

private:
    int m_screenWidth;
    int m_screenHeight;
    glm::mat4 m_pixelMatrix;
};