#pragma once
#include "./Effect.h"
#include <vector>
#include <memory>

class EffectManager {
public:

    void AddEffect(std::shared_ptr<IEffect> effect)
    {
        m_effects.push_back(effect);
    }

    void Update(float deltatime)
    {
        for (auto& effect : m_effects) {
            effect->Update(deltatime);
        }

        std::erase_if(m_effects, [](const std::shared_ptr<IEffect>& effect) { 
            return !effect->GetLifetime() > 0;
        });
    }

    const std::vector<std::shared_ptr<IEffect>>& GetEffects() const
    {
        return m_effects;
    }

    void Clear()
    {
        m_effects.clear();
    }

private:
    std::vector<std::shared_ptr<IEffect>> m_effects;
};