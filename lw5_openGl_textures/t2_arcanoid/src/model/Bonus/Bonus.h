#pragma once
#include "./IBonusAction.h"
#include <memory>
#include "../../common/Geometry.h"

class Bonus {
public:
    Bonus(Point3D position, std::shared_ptr<IBonusAction> bonusAction)
        : m_position(position), m_bonusAction(bonusAction) {}

    void Update(float deltatime) {
        m_position.z += SPEED * deltatime;
    }

    std::shared_ptr<IBonusAction> GetBonusAction() {
        return m_bonusAction;
    }

    Point3D GetPosition() const
    {
        return m_position;
    }

    float GetRadius() const
    {
        return RADIUS;
    }

private:
    std::shared_ptr<IBonusAction> m_bonusAction;
    Point3D m_position = {0, 0, 0};
    inline static const float RADIUS = 0.15;
    inline static const float SPEED = 1;
};