#pragma once

#include "../ShovelBonusAction.h"
#include "../StarBonusAction.h"
#include "../MedkitBonusAction.h"
#include "../MachinganBonusAction.h"
#include "../FreezeBonusAction.h"
#include "../HelmetBonusAction.h"
#include "../IBonusAction.h"
#include "../../../Map/Map.h"
#include "../../../Tank/Tank.h"

#include <random>
#include <memory>
#include <functional>
#include <unordered_map>

class BonusActionFactory {
public:
    BonusActionFactory(
        Map& map,
        std::vector<std::shared_ptr<Tank>>& enemies
    )
        : m_map(map)
        , m_enemies(enemies) 
    {
    }

    std::shared_ptr<IBonusAction> CreateBonusAction(BonusActionType type, std::shared_ptr<Tank> owner) 
    {
        return GetBonusActionByType(type, owner);
    }
private:
    Map& m_map;
    std::vector<std::shared_ptr<Tank>>& m_enemies;

    std::shared_ptr<IBonusAction> GetBonusActionByType(BonusActionType type, std::shared_ptr<Tank> owner) 
    {
        auto it = m_bonusCreatorsMap.find(type);
        
        if (it != m_bonusCreatorsMap.end()) {
            return it->second(owner);
        }

        throw std::invalid_argument("Unknown bonusActionType: " + (int)type);
    };

    using BonusCreator = std::function<std::shared_ptr<IBonusAction>(std::shared_ptr<Tank>)>;

    std::unordered_map<BonusActionType, BonusCreator> m_bonusCreatorsMap = 
    {
        {BonusActionType::Shovel, [this](std::shared_ptr<Tank> owner){ return std::make_shared<ShovelBonusAction>(m_map, owner); }},
        {BonusActionType::Star, [this](std::shared_ptr<Tank> owner){ return std::make_shared<StarBonusAction>(owner); }},
        {BonusActionType::Medkit, [this](std::shared_ptr<Tank> owner){ return std::make_shared<MedkitBonusAction>(owner); }},
        {BonusActionType::Machingan, [this](std::shared_ptr<Tank> owner){ return std::make_shared<MachinganBonusAction>(owner); }},
        {BonusActionType::Freeze, [this](std::shared_ptr<Tank> owner){ return std::make_shared<FreezeBonusAction>(m_enemies, owner); }},
        {BonusActionType::Helmet, [this](std::shared_ptr<Tank> owner){ return std::make_shared<HelmetBonusAction>(owner); }},
    };
};