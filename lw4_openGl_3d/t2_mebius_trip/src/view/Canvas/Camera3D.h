#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum class CameraMode
{
    TARGET,
    FPV
};

class Camera3D
{
public:
    Camera3D()
    {
        m_projectionMatrix = glm::perspective(glm::radians(m_fov), m_aspect, m_near, m_far);
        UpdateViewMatrix();
    }

    Camera3D(float fov, float aspect, float near = 0.1, float far = 1000)
    {
        SetPerspective(fov, aspect, near, far);
        UpdateViewMatrix();
    }

    Camera3D(float left, float right, float bottom, float top, float near = 0.1, float far = 1000)
    {
        SetOrthographic(left, right, bottom, top, near, far);
        UpdateViewMatrix();
    }

    const glm::mat4 &GetViewMatrix() const { return m_viewMatrix; }
    const glm::mat4 &GetProjectionMatrix() const { return m_projectionMatrix; }

    void SetPerspective(float fov, float aspect, float near = 0.1, float far = 1000)
    {
        m_projectionMatrix = glm::perspective(glm::radians(fov), aspect, near, far);
        m_fov = fov;
        m_aspect = aspect;
        m_near = near;
        m_far = far;
    }

    void SetOrthographic(float left, float right, float bottom, float top, float near = 0.1, float far = 1000)
    {
        m_projectionMatrix = glm::ortho(left, right, bottom, top, near, far);
    }

    void SetTargetMode(glm::vec3 target, float distance)
    {
        m_mode = CameraMode::TARGET;
        m_target = target;
        m_distance = glm::distance(m_position, m_target);

        // Вычисляем углы относительно цели
        glm::vec3 direction = glm::normalize(m_position - m_target);
        m_yaw = glm::degrees(atan2(direction.z, direction.x));
        m_pitch = glm::degrees(asin(direction.y));

        UpdateViewMatrix();
    }

    void SetFPVMode()
    {
    }

private:
    glm::vec3 m_position;
    glm::vec3 m_position;
    glm::vec3 m_forward = {0, 0, 1};
    glm::vec3 m_up = {0, 1, 0};
    glm::mat4 m_projectionMatrix;
    glm::mat4 m_viewMatrix;
    float m_pitch;
    float m_yaw;
    float m_fov = 70;
    float m_aspect = 16.0f / 9.0f;
    float m_near = 0.1;
    float m_far = 1000;
    CameraMode m_mode = CameraMode::FPV;

    void UpdateViewMatrix()
    {
        m_viewMatrix = glm::lookAt(m_position, m_position + m_forward, m_up);
    }
};