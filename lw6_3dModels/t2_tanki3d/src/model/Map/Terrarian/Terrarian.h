#pragma once
#include "../../../common/Geometry.h"
#include "../../Constants.h"

enum class TerrarianType {
    Ice,
    Dirt,
};

class Terrarian {
public:
    Terrarian(TerrarianType type, const Point3D& position)
    : m_position(position),
      m_type(type) {}

    Point3D GetPosition() const
    {
        return m_position;
    }

    TerrarianType GetType() const
    {
        return m_type;
    }

    static Size3D GetSize() {
        return Constants::TERRARIAN_SIZE;
    }
private:
    Point3D m_position;
    TerrarianType m_type;
};