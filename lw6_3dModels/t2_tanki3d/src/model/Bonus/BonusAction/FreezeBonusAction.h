#pragma once
#include "./BaseBonusAction.h"
#include <vector>
#include "../../Map/Map.h"

class FreezeBonusAction : public BaseBonusAction {
public:
    FreezeBonusAction(
        std::vector<std::shared_ptr<Tank>>& enemies,
        std::shared_ptr<Tank> owner
    )
        : BaseBonusAction(owner)
        , m_enemies(enemies)
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
        for (auto& enemy : m_enemies) {
            enemy->SetFreezed(true);
        }
    };

    void ReApply() override
    {
        for (auto& enemy : m_enemies) {
            enemy->SetFreezed(false);
        }
    }

    BonusActionType GetType() const override
    {
        return BonusActionType::Freeze;
    }

    bool IsActionEnd() const override
    {
        return lifetime <= 0;
    }

private:
    float m_prevTotalReloadTime;
    const float MACHINEGAN_RELOAD_TIME = 0.5;
    float lifetime = 5;
    std::vector<std::shared_ptr<Tank>>& m_enemies;
};