#pragma once
#include "./BaseBonusAction.h"
#include <vector>
#include "../../Map/Map.h"

class StarBonusAction : public BaseBonusAction {
public:
    StarBonusAction(
        std::shared_ptr<Tank> owner
    )
        : BaseBonusAction(owner)
    {
    }

    void Apply() override 
    {
        m_owner->SetLevel(m_owner->GetLevel() + 1);
    };

    BonusActionType GetType() const override
    {
        return BonusActionType::Star;
    }

private:
};