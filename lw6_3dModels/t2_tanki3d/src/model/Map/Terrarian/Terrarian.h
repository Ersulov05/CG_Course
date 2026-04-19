#pragma once
#include "../../../common/Geometry.h"
#include "../../Constants.h"
#include "../../Object/GameObject.h"

enum class TerrarianType {
    Ice,
    Dirt,
    Water,
};

class Terrarian : public GameObject {
public:
    Terrarian(TerrarianType type, const Point3D& position)
        : GameObject(position, Constants::TERRARIAN_SIZE)
        , m_type(type) 
    {
    }

    TerrarianType GetType() const
    {
        return m_type;
    }

    inline static float GetFriction(TerrarianType type) 
    {        
        switch (type) {
            case TerrarianType::Dirt:
                return 0.5;
            case TerrarianType::Ice:
                return 0.1;
            default:
                return 0.5;
        }
    }

    inline static float GetTraction(TerrarianType type) 
    {
        switch (type) {
            case TerrarianType::Dirt:
                return 0.7;
            case TerrarianType::Ice:
                return 0.15;
            default:
                return 0.5;
        }
    }

private:
    TerrarianType m_type;
};