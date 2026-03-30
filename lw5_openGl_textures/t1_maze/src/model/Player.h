#pragma once
#include "../common/Geometry.h"
#include "./Collision/BoxCollision.h"
#include <algorithm>
#include <unordered_map>
#include <functional>
#include <vector>

enum class Direction {
    Forward,
    Back,
    Left,
    Right
};

class Player
{
public:
    Player() {
        InitCollisions(8);
    }

    using OnChangeCollisionCallback = std::function<bool()>;

    void Move(Direction direction, float deltatime) {
        auto oldPos = m_position;
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
        UpdateCollisions();
        if (NotifyChangeCollisionCallbacks()) {
            m_position = oldPos;
            UpdateCollisions();
        }
    }

    Point3D GetPosition() const {
        return m_position;
    }

    Vector3D GetRotation() const {
        return m_rotation;
    }

    void Rotate(float x, float y, float z) {
        auto oldRotation = m_rotation;
        m_rotation = m_rotation + Vector3D(x, y, z);

        UpdateCollisions();
        if (NotifyChangeCollisionCallbacks()) {
            m_rotation = oldRotation;
            UpdateCollisions();
        }
    }

    std::vector<BoxCollision> GetCollisions() const {
        return m_collisions;
    }

    void OnChangeCollisionSubscribe(void *subscriber, OnChangeCollisionCallback callback)
    {
        m_changeCollisionCallbacks[subscriber] = callback;
    }

private:
    Point3D m_position = {0, 1.8, 2};
    Vector3D m_rotation = {0, 0, 0};

    std::vector<BoxCollision> m_collisions;
    std::unordered_map<void *, OnChangeCollisionCallback> m_changeCollisionCallbacks;

    const float MOVE_SPEED = 5;

    bool NotifyChangeCollisionCallbacks()
    {
        auto callbacks = m_changeCollisionCallbacks;
        for (auto &[subscriber, callback] : callbacks)
        {
            if (callback)
            {
                if (callback()) {
                    return true;
                }
            }
        }

        return false;
    }

    void InitCollisions(int count) {
        for (int i =0; i < count; ++i) {
            m_collisions.push_back(BoxCollision(GetBodyPosition(), {0.2, 1, 0.2}, GetBodyRotation(i * 90.0f / count)));
        }
    }

    Vector3D GetBodyRotation(float yAngle = 0) const {
        return {0, m_rotation.y + yAngle, 0};
    }

    Point3D GetBodyPosition() const {
        return {m_position.x, m_position.y / 2, m_position.z};
    }

    void UpdateCollisions() {
        auto count = m_collisions.size();

        for (int i = 0; i < count; ++i) {
            m_collisions[i].SetPosition(GetBodyPosition());
            m_collisions[i].SetRotation(GetBodyRotation(i * 90.0f / count));
        }
    }
};