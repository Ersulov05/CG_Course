#pragma once
#include <unordered_set>
#include <ostream>

enum class BonusActionType {
    Grenade, // Не придумал:)
    Helmet, // Даёт щит
    Shovel, // Ремонтирует стены главного штаба
    Star, // Повышает уровень танка
    Timer, // Замораживает врагов
    Medkit, // лечит танк
    Machingan, // ускоряет перезарядку
};

class IBonusAction {
public:
    virtual void Apply() = 0;
    virtual void ReApply() {};
    virtual void Update(float deltatime) {};
    virtual BonusActionType GetType() const = 0;
    virtual const std::shared_ptr<Tank> GetOwner() const = 0;
    virtual bool IsActionEnd() const 
    {
        return true;
    };
};

inline static const std::unordered_set<BonusActionType> ONLY_PLAYER_BONUS_ACTIONS = 
{
    BonusActionType::Shovel,
};

inline static bool IsOnlyPlayerBonusAction(BonusActionType type)
{
    return ONLY_PLAYER_BONUS_ACTIONS.find(type) != ONLY_PLAYER_BONUS_ACTIONS.end();
}