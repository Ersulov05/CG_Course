#pragma once
#include "./IBonusAction.h"
#include <memory>
#include <vector>
#include <functional>

class BonusActionManager {
public:
    void ApplyBonus(std::shared_ptr<IBonusAction> bonusAction) {
        RemoveConflictingBonuses(bonusAction);
        bonusAction->Apply();

        if (!CheckReApply(bonusAction)) {
            m_activeBonuses.push_back(bonusAction);
        }
    }

    void Update(float deltatime) {
        m_activeBonuses.erase(
            std::remove_if(m_activeBonuses.begin(), m_activeBonuses.end(),
                [&deltatime, this](std::shared_ptr<IBonusAction>& activeBonus) {
                    activeBonus->Update(deltatime);
                    return CheckReApply(activeBonus);
                }
            ),
            m_activeBonuses.end()
        );
    }

    void Clear() {
        for (auto& activeBonus : m_activeBonuses) {
            activeBonus->ReApply();
        }
        m_activeBonuses.clear();
    }
private:
    std::vector<std::shared_ptr<IBonusAction>> m_activeBonuses;

    bool CheckReApply(std::shared_ptr<IBonusAction> bonusAction) {
        if (bonusAction->IsActionEnd()) {
            bonusAction->ReApply();
            return true;
        }
        return false;
    }

    void RemoveConflictingBonuses(std::shared_ptr<IBonusAction>& newBonusAction) {
        auto newType = newBonusAction->GetType();
        
        m_activeBonuses.erase(
            std::remove_if(m_activeBonuses.begin(), m_activeBonuses.end(),
                [&newType, this](std::shared_ptr<IBonusAction>& activeBonus) {
                    if (IsConflicting(newType, activeBonus->GetType())) {
                        activeBonus->ReApply();
                        return true;
                    }
                    return false;
                }
            ),
            m_activeBonuses.end()
        );
    }

    bool IsConflicting(BonusActionType firstType, BonusActionType secondType)
    {
        for (auto& conflictPair : CONFLICTED_BONUS_TYPES) {
            if ((firstType == conflictPair.first && secondType == conflictPair.second) ||
                (firstType == conflictPair.second && secondType == conflictPair.first) ||
                (firstType == conflictPair.first && secondType == conflictPair.first) ||
                (firstType == conflictPair.second && secondType == conflictPair.second)
            ) {
                return true;
            }
        }
        return false;
    }

    inline static std::vector<std::pair<BonusActionType, BonusActionType>> CONFLICTED_BONUS_TYPES = {
        {BonusActionType::AddSizeBalls, BonusActionType::SubSizeBalls},
        {BonusActionType::AddSpeedBalls, BonusActionType::SubSpeedBalls},
        {BonusActionType::AddSizeRacket, BonusActionType::SubSizeRacket},
    };
};