#pragma once
#include "./IBonusAction.h"
#include "../../Tank/Tank.h"

class BaseBonusAction : public IBonusAction {
public:
    BaseBonusAction(std::shared_ptr<Tank> owner)
        : m_owner(owner)
    {
    }

    const std::shared_ptr<Tank> GetOwner() const
    {
        return m_owner;
    }

protected:
    std::shared_ptr<Tank> m_owner;
};