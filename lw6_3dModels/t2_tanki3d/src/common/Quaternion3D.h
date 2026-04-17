#pragma once
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include "./Point3D.h"

struct Quaternion3D {
    glm::quat m_quaternion;
    
    Quaternion3D() : m_quaternion(glm::quat(1.0f, 0.0f, 0.0f, 0.0f)) {}

    Quaternion3D(const glm::quat& quat) : m_quaternion(quat) {}
    
    Quaternion3D(float pitch, float yaw, float roll) {
        m_quaternion = glm::quat(glm::vec3(pitch, yaw, roll));
    }

    Quaternion3D(const Vector3D& axis, float angle) {
        m_quaternion = glm::angleAxis(angle, glm::vec3(axis.x, axis.y, axis.z));
    }

    static Quaternion3D FromDegrees(float pitch, float yaw, float roll) {
        constexpr float DEG_TO_RAD = 3.14159265359f / 180.0f;
        return Quaternion3D(
            pitch * DEG_TO_RAD,
            yaw * DEG_TO_RAD,
            roll * DEG_TO_RAD
        );
    }

    static Quaternion3D FromDirection(const Vector3D& direction) {
        // Находим угол между направлением и осью Z (0, 0, 1)
        Vector3D forward = Vector3D(0.0f, 0.0f, 1.0f);
        Vector3D dir = direction.Normalized();
        
        // Вычисляем угол поворота
        float dot = forward.Dot(dir);
        float angle = acos(dot);  // Угол в радианах
        
        // Если векторы противоположны
        if (angle < 0.0001f) {
            return Quaternion3D(0.0f, 0.0f, 0.0f);  // Нулевой поворот
        }
        
        if (angle > 3.14159f - 0.0001f) {
            return Quaternion3D(0.0f, 3.14159f, 0.0f);  // Разворот на 180 градусов
        }
        
        // Ось вращения = cross(forward, direction)
        Vector3D axis = forward.Cross(dir).Normalized();
        
        return Quaternion3D(axis, angle);
    }

    void Rotate(Quaternion3D quaternion) {
        m_quaternion = quaternion.m_quaternion * m_quaternion;
    }

    void RotateY(float angle) {
        glm::quat rotation = glm::angleAxis(angle, glm::vec3(0.0f, 1.0f, 0.0f));
        m_quaternion = rotation * m_quaternion;
    }

    void RotateX(float angle) {
        glm::quat rotation = glm::angleAxis(angle, glm::vec3(1.0f, 0.0f, 0.0f));
        m_quaternion = rotation * m_quaternion;
    }

    void RotateZ(float angle) {
        glm::quat rotation = glm::angleAxis(angle, glm::vec3(0.0f, 0.0f, 1.0f));
        m_quaternion = rotation * m_quaternion;
    }

    Vector3D GetEulerAngles() const {
        glm::vec3 euler = glm::eulerAngles(m_quaternion);
        return Vector3D(euler.x, euler.y, euler.z);
    }

    Vector3D GetForward() const {
        glm::vec3 forward = m_quaternion * glm::vec3(0.0f, 0.0f, 1.0f);
        return Vector3D(forward.x, forward.y, forward.z);
    }
    
    Vector3D GetRight() const {
        glm::vec3 right = m_quaternion * glm::vec3(1.0f, 0.0f, 0.0f);
        return Vector3D(right.x, right.y, right.z);
    }
    
    Vector3D GetUp() const {
        glm::vec3 up = m_quaternion * glm::vec3(0.0f, 1.0f, 0.0f);
        return Vector3D(up.x, up.y, up.z);
    }

    float GetYaw() const {
        return glm::yaw(m_quaternion);
    }
    
    // Получить угол поворота по оси X (в радианах)
    float GetPitch() const {
        return glm::pitch(m_quaternion);
    }
    
    // Получить угол поворота по оси Z (в радианах)
    float GetRoll() const {
        return glm::roll(m_quaternion);
    }

    void Normalize() {
        m_quaternion = glm::normalize(m_quaternion);
    }

    Point3D operator*(const Point3D& point) const 
    {
        glm::vec3 result = m_quaternion * glm::vec3(point.x, point.y, point.z);
        return Point3D(result.x, result.y, result.z);
    }

    Quaternion3D operator*(const Quaternion3D& other) const {
        return Quaternion3D(m_quaternion * other.m_quaternion);
    }

    operator glm::quat() const {
        return m_quaternion;
    }
};