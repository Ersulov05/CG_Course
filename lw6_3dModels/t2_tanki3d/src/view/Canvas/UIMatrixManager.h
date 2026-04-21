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

    const glm::mat4& GetPixelMatrix() const { return m_pixelMatrix; }

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