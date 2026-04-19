#pragma once
#include "./BaseBonusAction.h"
#include <vector>
#include "../../Map/Map.h"

class ShovelBonusAction : public BaseBonusAction {
public:
    ShovelBonusAction(
        Map& map,
        std::shared_ptr<Tank> owner
    )
        : BaseBonusAction(owner)
        , m_map(map)
    {
    }

    void Apply() override 
    {
        m_map.RepairHeadquartersWalls();
    };

    BonusActionType GetType() const override
    {
        return BonusActionType::Shovel;
    }

private:
    Map& m_map;
};