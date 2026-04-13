#pragma once
#include "../common/Geometry.h"

class Car {
public:
    Car(Point3D startPos): m_startPos(startPos), m_position(startPos) {
    }

    void Update(float deltatime) {
        m_position = m_position + m_speed * deltatime;
        if (m_position.z < m_startPos.z - DISTANCE) {
            m_position = m_startPos;
        }
    }

    Point3D GetPosition() const {
        return m_position;
    }
    
private:
    Point3D m_startPos = {1, 0, 7};
    Point3D m_position = m_startPos;
    Vector3D m_speed = {0, 0, -2};
    const float DISTANCE = 13;
};