#pragma once
#include "./Bonus.h"
#include "./BonusActionManager.h"
#include <vector>
#include "./Factory/BonusFactory.h"

class BonusManager
{
public:
    BonusManager(Map& map)
        : m_bonusFactory(map)
    {
        m_bonuses.push_back(m_bonusFactory.CreateBonus());
    }

    void Update(float deltatime)
    {
        std::erase_if(m_bonuses, [](const Bonus& bonus) { 
            return !bonus.IsAlive(); 
        });
    }

    const std::vector<Bonus>& GetBonuses() const
    {
        return m_bonuses;
    }

    std::vector<Bonus>& GetBonuses()
    {
        return m_bonuses;
    }

private:
    std::vector<Bonus> m_bonuses;
    BonusFactory m_bonusFactory;
};