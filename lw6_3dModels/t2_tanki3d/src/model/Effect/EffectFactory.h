#pragma once
#include "./Effect.h"
#include <memory>
#include <unordered_map>
#include <functional>

class EffectFactory {
public:
    static std::shared_ptr<IEffect> CreateEffect(EffectType type, const Point3D& position)
    {
        auto effect = GetEffectByType(type);

        return effect(position);
    }
private:
    using EffectCreator = std::function<std::shared_ptr<IEffect>(const Point3D&)>;

    static EffectCreator GetEffectByType(EffectType type) 
    {
        auto it = m_effectCreatorsMap.find(type);
        
        if (it != m_effectCreatorsMap.end()) {
            return it->second;
        }

        throw std::invalid_argument("Unknown effectType: " + (int)type);
    };

    inline static const std::unordered_map<EffectType, EffectCreator> m_effectCreatorsMap = 
    {
        {EffectType::Boom, [](const Point3D& position){ return std::make_shared<BoomEffect>(position); }},
    };
};