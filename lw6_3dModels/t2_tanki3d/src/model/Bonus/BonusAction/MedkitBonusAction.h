#pragma once
#include "./BaseBonusAction.h"
#include <vector>
#include "../../Map/Map.h"

class MedkitBonusAction : public BaseBonusAction {
public:
    MedkitBonusAction(
        std::shared_ptr<Tank> owner
    )
        : BaseBonusAction(owner)
    {
    }

    void Apply() override 
    {
        m_owner->AddHealth(HEALTH_COUNT);
    };

    BonusActionType GetType() const override
    {
        return BonusActionType::Medkit;
    }

private:
    const unsigned int HEALTH_COUNT = 250;
};