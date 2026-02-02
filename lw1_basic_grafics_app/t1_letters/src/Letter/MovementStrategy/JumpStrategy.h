#pragma once

#include "../../Common/Point.h"
#include "./IMovementStrategy.h"
#include <vector>

class JumpStrategy : public IMovementStrategy
{
public:
    JumpStrategy(float startJump = 0) : m_jump(startJump)
    {
        if (startJump < 0)
        {
            m_jump = 0;
        }
        else if (startJump > MAX_JUMP)
        {
            m_jump = MAX_JUMP;
            jump_acc *= -1;
        }
    }

    void Move() override
    {
        m_jump += jump_acc;

        if (m_jump > MAX_JUMP || m_jump < 0)
        {
            jump_acc *= -1;
        }
    }

    const Point GetPosition() const override
    {
        return {0, m_jump};
    }

private:
    float m_jump = 0;
    float jump_acc = 0.5;
    static const int MAX_JUMP = 200;
};