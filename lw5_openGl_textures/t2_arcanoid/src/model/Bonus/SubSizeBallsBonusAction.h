#pragma once
#include "./IBonusAction.h"
#include <vector>
#include "../Ball.h"

class SubSizeBallsBonusAction : public IBonusAction {
public:
    SubSizeBallsBonusAction(std::vector<Ball>& balls): m_balls(balls)
    {
    }

    void Apply() override 
    {
        for (auto& ball: m_balls) {
            ball.SetRadius(ball.GetRadius() / SUBBED_SIZE_COEF);
        }
    };

    void ReApply() override 
    {
        for (auto& ball: m_balls) {
            ball.SetRadius(ball.GetRadius() * SUBBED_SIZE_COEF);
        }
    };

    BonusActionType GetType() const override
    {
        return BonusActionType::SubSizeBalls;
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
    float lifetime = 7.5;
    const float SUBBED_SIZE_COEF = 2;
};