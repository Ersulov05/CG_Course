#pragma once
#include "../Canvas/ICanvas3D.h"
#include "../../model/Effect/Effect.h"
#include "./BoomEffectView.h"

class EffectView {
public:
    static void Draw(ICanvas3D& canvas, const std::shared_ptr<IEffect>& effect)
    {
        if (effect->GetLifetime() < 0) return;
        
        switch (effect->GetType()) {
            case EffectType::Boom:
                BoomEffectView::Draw(canvas, effect);
                break;
        }
    }

private:
};