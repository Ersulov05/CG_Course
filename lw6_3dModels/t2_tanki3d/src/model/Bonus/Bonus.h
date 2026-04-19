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

    void Update(float deltatime)
    {
        if (IsAlive()) {
            m_lifetime -= deltatime;
        }
    }

    BonusActionType GetType() const
    {
        return m_bonusActionType;
    }

    bool IsAlive() const
    {
        return m_lifetime > 0;
    }

    void Boom()
    {
        m_lifetime = 0;
    }

private:
    BonusActionType m_bonusActionType;
    float m_lifetime = Constants::BONUS_LIFETIME;
};