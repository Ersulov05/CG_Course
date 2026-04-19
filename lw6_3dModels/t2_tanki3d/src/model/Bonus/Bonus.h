#pragma once
#include "./BonusAction/IBonusAction.h"
#include "../../common/Geometry.h"
#include "../Object/GameObject.h"
#include "../Constants.h"

class Bonus : public GameObject {
public:
    Bonus(Point3D position, BonusActionType bonusActionType)
        : GameObject(position, Constants::BONUS_SIZE)
        , m_bonusActionType(bonusActionType) 
    {
    }

    BonusActionType GetType() const
    {
        return m_bonusActionType;
    }

    bool IsAlive() const
    {
        return isAlive;
    }

    void Boom()
    {
        isAlive = false;
    }

private:
    BonusActionType m_bonusActionType;
    bool isAlive = true;
};