#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <iostream>
#include "../../common/Geometry.h"

class FpvCamera3D
{
public:
    FpvCamera3D()
        : m_position(0.0f, 0.0f, 5.0f),
          m_rotation(glm::quat(1, 0, 0, 0)),
          m_fov(70.0f),
          m_aspect(16.0f / 9.0f),
          m_near(0.1f),
          m_far(1000.0f),
          m_uiWidth(800),
          m_uiHeight(600)
    {
        UpdateViewMatrix();
        UpdateProjectionMatrix();
    }

    const glm::mat4& GetViewMatrix() const { return m_viewMatrix; }
    const glm::mat4& GetProjectionMatrix() const { return m_projectionMatrix; }

    void SetPosition(const glm::vec3& position)
    {
        m_position = position;
        UpdateViewMatrix();
    }

    void SetPosition(float x, float y, float z)
    {
        m_position = glm::vec3(x, y, z);
        UpdateViewMatrix();
    }

    void SetPosition(Point3D position)
    {
        SetPosition(position.x, position.y, position.z);
    }

    void SetRotation(Vector3D rotation)
    {
        SetRotation(rotation.x, rotation.y, rotation.z);
    }

    void SetRotation(float yawDegrees, float pitchDegrees, float rollDegrees = 0.0f)
    {
        glm::quat qYaw = glm::angleAxis(glm::radians(yawDegrees), glm::vec3(0, 1, 0));
        glm::quat qPitch = glm::angleAxis(glm::radians(pitchDegrees), glm::vec3(1, 0, 0));
        glm::quat qRoll = glm::angleAxis(glm::radians(rollDegrees), glm::vec3(0, 0, 1));
        
        m_rotation = qYaw * qPitch * qRoll;
        UpdateViewMatrix();
    }

    Point3D GetPosition() const { return m_position; }
    Quaternion3D GetRotation() const { return m_rotation; }

    glm::vec3 GetForward() const
    {
        return glm::rotate(m_rotation, glm::vec3(0, 0, -1));
    }

    glm::vec3 GetRight() const
    {
        return glm::rotate(m_rotation, glm::vec3(1, 0, 0));
    }

    glm::vec3 GetUp() const
    {
        return glm::rotate(m_rotation, glm::vec3(0, 1, 0));
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
    void UpdateViewMatrix()
    {
        glm::vec3 forward = GetForward();
        glm::vec3 target = m_position + forward;
        glm::vec3 up = GetUp();
        
        m_viewMatrix = glm::lookAt(m_position, target, up);
    }

    void UpdateProjectionMatrix()
    {
        m_projectionMatrix = glm::perspective(glm::radians(m_fov), m_aspect, m_near, m_far);
    }

private:
    glm::vec3 m_position;
    glm::quat m_rotation;

    glm::mat4 m_viewMatrix;
    glm::mat4 m_projectionMatrix;

    float m_fov;
    float m_aspect;
    float m_near;
    float m_far;

    float m_uiWidth;
    float m_uiHeight;
};