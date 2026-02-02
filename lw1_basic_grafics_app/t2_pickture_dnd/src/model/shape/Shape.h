#pragma once

#include "./IShape.h"

class Shape : public IShape
{
public:
    Shape(unsigned id, Color fillColor, Color outlineColor)
        : m_id(id),
          m_fillColor(fillColor),
          m_outlineColor(outlineColor) {}

    Color GetFillColor() const override
    {
        return m_fillColor;
    }

    Color GetOutlineColor() const override
    {
        return m_outlineColor;
    }

    unsigned GetId() const override
    {
        return m_id;
    }

private:
    unsigned m_id;
    Color m_fillColor;
    Color m_outlineColor;
};