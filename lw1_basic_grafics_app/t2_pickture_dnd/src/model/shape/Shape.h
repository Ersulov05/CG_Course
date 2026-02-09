#pragma once

#include "./IShape.h"

class Shape : public IShape
{
public:
    Shape(Color fillColor, Color outlineColor)
        : m_fillColor(fillColor),
          m_outlineColor(outlineColor) {}

    Color GetFillColor() const override
    {
        return m_fillColor;
    }

    Color GetOutlineColor() const override
    {
        return m_outlineColor;
    }

private:
    Color m_fillColor;
    Color m_outlineColor;
};