#pragma once
#include "./BaseBonusAction.h"
#include <vector>
#include "../../Map/Map.h"

class MachinganBonusAction : public BaseBonusAction {
public:
    MachinganBonusAction(
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
        auto& cannon = m_owner->GetCannon();

        m_prevTotalReloadTime = cannon.GetTotalReloadTime();
        cannon.SetTotalReloadTime(MACHINEGAN_RELOAD_TIME);
    };

    void ReApply() override
    {
        auto& cannon = m_owner->GetCannon();

        cannon.SetTotalReloadTime(m_prevTotalReloadTime);
    }

    BonusActionType GetType() const override
    {
        return BonusActionType::Machingan;
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