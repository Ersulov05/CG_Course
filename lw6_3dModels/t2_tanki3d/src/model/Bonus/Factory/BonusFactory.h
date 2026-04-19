#pragma once
#include "../../Map/Map.h"
#include "../Bonus.h"
#include "../BonusAction/IBonusAction.h"
#include "../../Constants.h"

#include <random>

class BonusFactory {
public:
    BonusFactory(Map& map)
        : m_map(map) 
    {
    }

    Bonus CreateBonus() 
    {
        return Bonus(
            GetRandomBonusPosition(),
            GetRandomBonusActionType()
        );
    }
private:
    Map& m_map;
    std::mt19937 m_randomEngine = std::mt19937(std::random_device{}());;

    BonusActionType GetRandomBonusActionType()
    {
        std::uniform_int_distribution<size_t> dist(0, m_bonusActions.size() - 1);
        size_t randomIndex = dist(m_randomEngine);
        
        return m_bonusActions[randomIndex];
    }

    Point3D GetRandomBonusPosition()
    {
        float mapWidth = m_map.GetWidth() - Constants::TERRARIAN_SIZE.width;
        float mapHeight = m_map.GetHeight() - Constants::TERRARIAN_SIZE.depth;

        std::uniform_real_distribution<float> distX(-mapWidth/2, mapWidth/2);
        std::uniform_real_distribution<float> distZ(-mapHeight/2, mapHeight/2);
        
        return Point3D{distX(m_randomEngine), Constants::BONUS_SIZE.height/2, distZ(m_randomEngine)};
    }

    std::vector<BonusActionType> m_bonusActions = {
        BonusActionType::Shovel,
        BonusActionType::Star,
        BonusActionType::Medkit,
        BonusActionType::Machingan,
    };
};