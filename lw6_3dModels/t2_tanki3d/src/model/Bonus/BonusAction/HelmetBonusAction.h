#pragma once
#include "./BaseBonusAction.h"
#include <vector>
#include "../../Map/Map.h"

class HelmetBonusAction : public BaseBonusAction {
public:
    HelmetBonusAction(
        std::shared_ptr<Tank> owner
    )
        : BaseBonusAction(owner)
    {
    }

    void Update(float deltatime) override
    {
        if (!IsActionEnd()) {
            lifetime -= deltatime;
        }
    }

    void Apply() override 
    {
        m_owner->SetProtected(true);
    };

    void ReApply() override
    {
        m_owner->SetProtected(false);
    }

    BonusActionType GetType() const override
    {
        return BonusActionType::Helmet;
    }

    bool IsActionEnd() const override
    {
        return lifetime <= 0;
    }

private:
    float m_prevTotalReloadTime;
    const float MACHINEGAN_RELOAD_TIME = 0.5;
    float lifetime = 5;
};