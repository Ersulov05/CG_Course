#pragma once
#include "./BonusAction/IBonusAction.h"
#include <memory>
#include <vector>
#include <functional>
#include <unordered_map>
#include <algorithm>

class BonusActionManager {
public:
    using BonusOwner = std::shared_ptr<Tank>;
    using BonusList = std::vector<std::shared_ptr<IBonusAction>>;
    using BonusMap = std::unordered_map<BonusOwner, BonusList>;

    void ApplyBonus(std::shared_ptr<IBonusAction> bonusAction) {
        auto owner = bonusAction->GetOwner();
        if (!owner) return;

        RemoveConflictingBonuses(bonusAction);
        bonusAction->Apply();

        if (!CheckAndHandleReApply(bonusAction)) {
            m_activeBonusesMap[owner].push_back(bonusAction);
        }
    }

    void Update(float deltatime) {
        for (auto& [owner, activeBonuses] : m_activeBonusesMap) {
            for (auto& activeBonus : activeBonuses) {
                activeBonus->Update(deltatime);
            }
        }

        std::erase_if(m_activeBonusesMap, [this](auto& pair) { 
            auto& [owner, activeBonuses] = pair;
            std::erase_if(activeBonuses, [this](std::shared_ptr<IBonusAction>& activeBonus){
                return CheckAndHandleReApply(activeBonus);
            });
            return activeBonuses.empty(); 
        });
    }

    void Clear() {
        for (auto& [owner, activeBonuses] : m_activeBonusesMap) {
            for (auto& activeBonus : activeBonuses) {
                activeBonus->ReApply();
            }
        }
        
        m_activeBonusesMap.clear();
    }

    const BonusList GetBonuses(const BonusOwner owner) const
    {
        auto it = m_activeBonusesMap.find(owner);
        if (it == m_activeBonusesMap.end()) {
            return it->second;
        }
        return {};
    }
private:
    BonusMap m_activeBonusesMap;

    bool CheckAndHandleReApply(std::shared_ptr<IBonusAction> bonusAction) {
        if (bonusAction->IsActionEnd()) {
            bonusAction->ReApply();
            return true;
        }
        return false;
    }

    void RemoveConflictingBonuses(std::shared_ptr<IBonusAction>& newBonusAction) {
        auto newType = newBonusAction->GetType();

        auto it = m_activeBonusesMap.find(newBonusAction->GetOwner());
        if (it == m_activeBonusesMap.end()) {
            return;
        }
        auto& activeBonuses = it->second;

        std::erase_if(activeBonuses, [&newType, this](std::shared_ptr<IBonusAction>& activeBonus) {
            if (IsConflicting(newType, activeBonus->GetType())) {
                activeBonus->ReApply();
                return true;
            }
            return false;
        });
    }

    bool IsConflicting(BonusActionType firstType, BonusActionType secondType)
    {
        if (firstType == secondType) return true;
        for (auto& conflictPair : CONFLICTED_BONUS_TYPES) {
            if ((firstType == conflictPair.first && secondType == conflictPair.second) ||
                (firstType == conflictPair.second && secondType == conflictPair.first)
            ) {
                return true;
            }
        }
        return false;
    }

    inline static std::vector<std::pair<BonusActionType, BonusActionType>> CONFLICTED_BONUS_TYPES = 
    {
        {BonusActionType::Machingan, BonusActionType::Star}
    };
};