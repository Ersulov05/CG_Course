#pragma once
#include "./IBonusAction.h"
#include <vector>
#include "../Ball.h"

class SubSpeedBallsBonusAction : public IBonusAction {
public:
    SubSpeedBallsBonusAction(std::vector<Ball>& balls): m_balls(balls)
    {
    }

    void Apply() override 
    {
        for (auto& ball: m_balls) {
            ball.SetSpeed(ball.GetSpeed() / SUBBED_SPEED_COEF);
        }
    };

    void ReApply() override 
    {
        for (auto& ball: m_balls) {
            ball.SetSpeed(ball.GetSpeed() * SUBBED_SPEED_COEF);
        }
    };

    BonusActionType GetType() const override
    {
        return BonusActionType::SubSpeedBalls;
    }

    void Update(float deltatime) override
    {
        lifetime -= deltatime;
    }

    bool IsActionEnd() const override
    {
        return lifetime < 0;
    }

private:
    std::vector<Ball>& m_balls;
    float lifetime = 5;
    const float SUBBED_SPEED_COEF = 2;
};