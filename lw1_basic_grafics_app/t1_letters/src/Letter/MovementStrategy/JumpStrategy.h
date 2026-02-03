#pragma once

#include "../../Common/Point.h"
#include "./IMovementStrategy.h"
#include <vector>

const unsigned PIXEL_PER_METER = 100;

class JumpStrategy : public IMovementStrategy
{
public:
    JumpStrategy(float startJumpPos = 0) : m_yPos(startJumpPos)
    {
    }

    void Update(float deltaTime) override
    {
        m_jumpSpeed += JUMP_G * deltaTime;
        m_yPos += m_jumpSpeed * PIXEL_PER_METER * deltaTime;

        if (m_yPos >= 0)
        {
            m_yPos = 0;
            m_jumpSpeed = -START_JUMP_SPEED;
        }
    }

    const Point GetPosition() const override
    {
        return {0, m_yPos};
    }

private:
    float m_yPos = 0;
    float m_jumpSpeed = 0;
    static constexpr float START_JUMP_SPEED = 5;
    static constexpr float JUMP_G = 9.8;
};