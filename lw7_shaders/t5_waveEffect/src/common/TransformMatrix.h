#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "./Vector3D.h"

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

    void Translate(float x, float y, float z)
    {
        m_matrix = glm::translate(m_matrix, glm::vec3(x, y, z));
    }

    void Translate(Vector3D vec)
    {
        m_matrix = glm::translate(m_matrix, glm::vec3(vec.x, vec.y, vec.z));
    }

    void Rotate(const glm::quat& quat)
    {
        m_matrix = m_matrix * glm::mat4_cast(quat);
    }

    void RotateX(float angleDegrees)
    {
        m_matrix = glm::rotate(m_matrix, glm::radians(angleDegrees), glm::vec3(1.0f, 0.0f, 0.0f));
    }

    void RotateY(float angleDegrees)
    {
        m_matrix = glm::rotate(m_matrix, glm::radians(angleDegrees), glm::vec3(0.0f, 1.0f, 0.0f));
    }

    void RotateZ(float angleDegrees)
    {
        m_matrix = glm::rotate(m_matrix, glm::radians(angleDegrees), glm::vec3(0.0f, 0.0f, 1.0f));
    }

    void Scale(float scaleX, float scaleY, float scaleZ)
    {
        m_matrix = glm::scale(m_matrix, glm::vec3(scaleX, scaleY, scaleZ));
    }

    void Scale(float scale)
    {
        Scale(scale, scale, scale);
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

    Point3D Apply(const Point3D &p) const
    {
        glm::vec4 result = m_matrix * glm::vec4(p.x, p.y, p.z, 1.0f);
        return Point3D(result.x, result.y, result.z);
    }

private:
    glm::mat4 m_matrix;
};