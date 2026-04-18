#pragma once
#include "../../../common/Geometry.h"
#include "../../Constants.h"
#include "../../Object/GameObject.h"

enum class TerrarianType {
    Ice,
    Dirt,
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

private:
    TerrarianType m_type;
};