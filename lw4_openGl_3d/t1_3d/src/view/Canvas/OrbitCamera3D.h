#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <cmath>

class OrbitCamera3D
{
public:
    OrbitCamera3D()
        : m_target(0.0f, 0.0f, 0.0f),
          m_distance(5.0f),
          m_fov(70.0f),
          m_aspect(16.0f / 9.0f),
          m_near(0.1f),
          m_far(1000.0f)
    {
        UpdatePosition();
        UpdateProjectionMatrix();
        UpdateViewMatrix();
    }

    const glm::mat4 &GetViewMatrix() const { return m_viewMatrix; }
    const glm::mat4 &GetProjectionMatrix() const { return m_projectionMatrix; }
    const glm::vec3& GetPosition() const { return m_position; }

    void Rotate(float deltaYaw, float deltaPitch, float rollDegrees = 0.0f)
    {
        glm::quat qYaw = glm::angleAxis(glm::radians(deltaYaw), glm::vec3(0, 1, 0));
        glm::quat qPitch = glm::angleAxis(glm::radians(deltaPitch), glm::vec3(1, 0, 0));
        glm::quat qRoll = glm::angleAxis(glm::radians(rollDegrees), glm::vec3(0, 0, 1));

        m_rotation = m_rotation * qYaw * qPitch * qRoll;
        m_rotation = glm::normalize(m_rotation);

        UpdatePosition();
        UpdateViewMatrix();
    }

    void Zoom(float deltaZoom)
    {
        m_distance = glm::max(0.1f, m_distance + deltaZoom);
        UpdatePosition();
        UpdateViewMatrix();
    }

    void SetDistance(float distance)
    {
        m_distance = glm::max(0.1f, distance);
        UpdatePosition();
        UpdateViewMatrix();
    }

    void SetPerspective(float fov, float aspect, float near = 0.1f, float far = 1000.0f)
    {
        m_fov = fov;
        m_aspect = aspect;
        m_near = near;
        m_far = far;
        UpdateProjectionMatrix();
    }

    void SetAspect(float aspect)
    {
        m_aspect = aspect;
        UpdateProjectionMatrix();
    }    

private:
    glm::vec3 m_position;
    glm::vec3 m_target; 

    float m_distance;

    glm::mat4 m_projectionMatrix;
    glm::mat4 m_viewMatrix;
    glm::quat m_rotation = glm::quat(1, 0, 0, 0);

    float m_fov;
    float m_aspect;
    float m_near;
    float m_far;

    void UpdatePosition()
    {
        glm::vec3 localPos(0.0f, 0.0f, m_distance);
        m_position = m_target + glm::rotate(m_rotation, localPos);
    }

    void UpdateViewMatrix()
    {
        glm::vec3 up = glm::rotate(m_rotation, glm::vec3(0, 1, 0));
        m_viewMatrix = glm::lookAt(m_position, m_target, up);
    }

    void UpdateProjectionMatrix()
    {
        m_projectionMatrix = glm::perspective(glm::radians(m_fov), m_aspect, m_near, m_far);
    }
};