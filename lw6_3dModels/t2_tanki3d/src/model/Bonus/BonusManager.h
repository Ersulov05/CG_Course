#pragma once
#include "./Bonus.h"
#include "./BonusActionManager.h"
#include "./Factory/BonusFactory.h"
#include "../Collision/CollisionDetector.h"
#include "../Constants.h"
#include <vector>

class BonusManager
{
public:
    BonusManager(Map& map)
        : m_bonusFactory(map)
        , m_map(map)
    {
    }

    void Update(float deltatime)
    {
        for (auto& bonus : m_bonuses) 
        {
            bonus.Update(deltatime);
        }

        if (m_bonuses.size() < Constants::MAX_BONUSES) {
            if (m_bonusRespawnTime > 0) {
                m_bonusRespawnTime -= deltatime;
            } else {
                SpawnBonus();
            }
        }

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

    void Clear()
    {
        m_bonuses.clear();
    }

private:
    std::vector<Bonus> m_bonuses;
    float m_bonusRespawnTime = Constants::BONUS_RESPAWN_TIME;
    BonusFactory m_bonusFactory;
    Map& m_map;

    void SpawnBonus()
    {
        auto bonus = m_bonusFactory.CreateBonus();
        if (CheckCollisionSpawnedBonus(bonus)) 
        {
            return;
        }
        m_bonuses.push_back(bonus);
        m_bonusRespawnTime = Constants::BONUS_RESPAWN_TIME;
    }

    bool CheckCollisionSpawnedBonus(const Bonus& bonus)
    {
        for (auto& wall : m_map.GetWalls()) {
            if (CollisionDetector::Detect(bonus, wall)) {
                return true;
            }
        }
        for (auto& wall : m_map.GetHeadquartersWalls()) {
            if (CollisionDetector::Detect(bonus, wall)) {
                return true;
            }
        }
        if (CollisionDetector::Detect(bonus, m_map.GetHeadquarters()))
        {
            return true;
        }

        return false;
    }
};