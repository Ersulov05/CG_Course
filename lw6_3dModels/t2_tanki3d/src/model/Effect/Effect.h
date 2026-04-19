#pragma once
#include "../../common/Geometry.h"

enum class EffectType
{
    Boom,
};

class IEffect {
public:
    virtual void Update(float deltatime) = 0;
    virtual float GetLifetime() const = 0;
    virtual float GetTotalLifetime() const = 0;
    virtual EffectType GetType() const = 0;
    virtual Point3D GetPosition() const = 0;
private:
};

class Effect : public IEffect {
public:
    Effect(float lifetime)
        : m_lifetime(lifetime)
        , m_totalLifetime(lifetime) 
    {
    }

    void Update(float deltatime) override
    {
        if (GetLifetime() > 0) {
            m_lifetime -= deltatime;
        }
    }

    float GetLifetime() const override
    {
        return m_lifetime;
    }

    float GetTotalLifetime() const override
    {
        return m_totalLifetime;
    }
private:
    float m_lifetime;
    float m_totalLifetime;
};



class BoomEffect : public Effect {
public:
    BoomEffect(const Point3D& position, float lifetime = 2)
        : Effect(lifetime)
        , m_position(position)
    {
    }

    Point3D GetPosition() const override
    {
        return m_position;
    }

    EffectType GetType() const override
    {
        return EffectType::Boom;
    }
private:
    Point3D m_position;
};