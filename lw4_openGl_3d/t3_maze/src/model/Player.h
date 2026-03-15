#pragma once
#include "../common/Geometry.h"

enum class Direction {
    Forward,
    Back,
    Left,
    Right
};

class Player
{
public:
    void Move(Direction direction, float deltatime) {
        switch (direction) {
            case Direction::Forward:
                m_position.x -= sin(m_rotation.y / 180 * M_PI) * MOVE_SPEED * deltatime;
                m_position.z -= cos(m_rotation.y/ 180 * M_PI) * MOVE_SPEED * deltatime;
                break;

            case Direction::Back:
                m_position.x += sin(m_rotation.y/ 180 * M_PI) * MOVE_SPEED * deltatime;
                m_position.z += cos(m_rotation.y/ 180 * M_PI) * MOVE_SPEED * deltatime;
                break;
                
            case Direction::Left:
                m_position.x -= cos(m_rotation.y/ 180 * M_PI) * MOVE_SPEED * deltatime;
                m_position.z += sin(m_rotation.y/ 180 * M_PI) * MOVE_SPEED * deltatime;
                break;
            
            case Direction::Right:
                m_position.x += cos(m_rotation.y/ 180 * M_PI) * MOVE_SPEED * deltatime;
                m_position.z -= sin(m_rotation.y/ 180 * M_PI) * MOVE_SPEED * deltatime;
                break;
        }
    }

    Point3D GetPosition() {
        return m_position;
    }

    Vector3D GetRotation() {
        return m_rotation;
    }

    void Rotate(float x, float y, float z) {
        m_rotation = m_rotation + Vector3D(x, y, z);
    }

private:
    Point3D m_position = {0, 1.8, 2};
    Vector3D m_rotation = {0, 0, 0};

    const float MOVE_SPEED = 5;
};