#pragma once
#include "../../Ball.h"
#include "../../Racket.h"
#include "../Bonus.h"

#include "../AddSizeRacketBonusAction.h"
#include "../SubSizeRacketBonusAction.h"
#include "../AddSizeBallsBonusAction.h"
#include "../SubSizeBallsBonusAction.h"
#include "../AddSpeedBallsBonusAction.h"
#include "../SubSpeedBallsBonusAction.h"
#include "../SplitBallsBonusAction.h"
#include "../IBonusAction.h"

#include <random>
#include <memory>
#include <functional>

class BonusFactory {
public:
    BonusFactory(std::vector<Ball>& balls, Racket& racket)
        : m_balls(balls), m_racket(racket) {}

    Bonus CreateBonus(const Point3D& position) {
        Bonus newBonus(
            position,
            GetRandomBonusAction()
        );

        return newBonus;
    }
private:
    std::vector<Ball>& m_balls;
    Racket& m_racket;
    std::mt19937 m_randomEngine;

    std::shared_ptr<IBonusAction> GetRandomBonusAction()
    {
        std::uniform_int_distribution<size_t> dist(0, m_bonusCreators.size() - 1);
        size_t randomIndex = dist(m_randomEngine);
        
        return m_bonusCreators[randomIndex]();
    }

    std::vector<std::function<std::shared_ptr<IBonusAction>()>> m_bonusCreators = {
        [this]() { return std::make_shared<AddSizeRacketBonusAction>(m_racket); },
        [this]() { return std::make_shared<SubSizeRacketBonusAction>(m_racket); },
        [this]() { return std::make_shared<AddSizeBallsBonusAction>(m_balls); },
        [this]() { return std::make_shared<SubSizeBallsBonusAction>(m_balls); },
        [this]() { return std::make_shared<AddSpeedBallsBonusAction>(m_balls); },
        [this]() { return std::make_shared<SubSpeedBallsBonusAction>(m_balls); },
        [this]() { return std::make_shared<SplitBallsBonusAction>(m_balls); }
    };
};