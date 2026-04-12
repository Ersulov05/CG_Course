#pragma once
#include "./IBonusAction.h"
#include <vector>
#include "../Racket.h"

class SubSizeRacketBonusAction : public IBonusAction {
public:
    SubSizeRacketBonusAction(Racket& racket): m_racket(racket)
    {
    }

    void Apply() override 
    {
        Size3D newSize = m_racket.GetSize();
        newSize.width /= SUBBED_SIZE_COEF;
        m_racket.SetSize(newSize);
    };

    void ReApply() override 
    {
        Size3D newSize = m_racket.GetSize();
        newSize.width *= SUBBED_SIZE_COEF;
        m_racket.SetSize(newSize);
    };

    BonusActionType GetType() const override
    {
        return BonusActionType::SubSizeRacket;
    }

    void Update(float deltatime) override
    {
        lifetime -= deltatime;
    }

    bool IsActionEnd() const override
    {
        return lifetime < 0;
    }

private:
    Racket& m_racket;
    float lifetime = 5;
    const float SUBBED_SIZE_COEF = 2;
};