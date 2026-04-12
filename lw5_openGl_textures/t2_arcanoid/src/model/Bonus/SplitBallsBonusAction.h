#pragma once
#include "./IBonusAction.h"
#include <vector>
#include "../Ball.h"

class SplitBallsBonusAction : public IBonusAction {
public:
    SplitBallsBonusAction(std::vector<Ball>& balls): m_balls(balls)
    {
    }

    void Apply() override 
    {
        auto copyBalls = m_balls;

        for (auto& ball: copyBalls) {
            Vector3D moveDirection = ball.GetMoveDirection();
            auto leftBall = ball;
            auto rightBall = ball;

            leftBall.SetMoveDirection(GetRotatedMoveDirection(moveDirection, -ANGLE));
            rightBall.SetMoveDirection(GetRotatedMoveDirection(moveDirection, ANGLE));
            m_balls.push_back(leftBall);
            m_balls.push_back(rightBall);
        }
    };

    BonusActionType GetType() const override
    {
        return BonusActionType::SplitBalls;
    }

private:
    std::vector<Ball>& m_balls;
    const float ANGLE = 40;

    Vector3D GetRotatedMoveDirection(Vector3D moveDirection, float angle) {
        float rad = angle * M_PI / 180.0f;
        float cosA = cos(rad);
        float sinA = sin(rad);
        
        float x = moveDirection.x * cosA + moveDirection.z * sinA;
        float z = moveDirection.z * cosA - moveDirection.x * sinA;
        
        return Vector3D(x, moveDirection.y, z);
    }
};