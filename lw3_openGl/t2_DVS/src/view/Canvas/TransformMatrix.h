#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../Common/Point.h"

class TransformMatrix
{
public:
    TransformMatrix()
    {
        Reset();
    }

    void Reset()
    {
        m_matrix = glm::mat4(1.0f);
    }

    void Translate(float x, float y)
    {
        m_matrix = glm::translate(m_matrix, glm::vec3(x, y, 0.0f));
    }

    void Translate(const Point &p)
    {
        Translate(p.x, p.y);
    }

    void Rotate(float angleDegrees)
    {
        m_matrix = glm::rotate(m_matrix, glm::radians(angleDegrees), glm::vec3(0.0f, 0.0f, 1.0f));
    }

    void Scale(float scaleX, float scaleY)
    {
        m_matrix = glm::scale(m_matrix, glm::vec3(scaleX, scaleY, 1.0f));
    }

    void Scale(float scale)
    {
        Scale(scale, scale);
    }

    void Scale(const Point &scale)
    {
        Scale(scale.x, scale.y);
    }

    const float *GetMatrix() const
    {
        return glm::value_ptr(m_matrix);
    }

    const glm::mat4 &GetGLMMatrix() const
    {
        return m_matrix;
    }

    TransformMatrix operator*(const TransformMatrix &other) const
    {
        TransformMatrix result;
        result.m_matrix = m_matrix * other.m_matrix;
        return result;
    }

    Point Apply(const Point &p) const
    {
        glm::vec4 result = m_matrix * glm::vec4(p.x, p.y, 0.0f, 1.0f);
        return Point(result.x, result.y);
    }

private:
    glm::mat4 m_matrix;
};