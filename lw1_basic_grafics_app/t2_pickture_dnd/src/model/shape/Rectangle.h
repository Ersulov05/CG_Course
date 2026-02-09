#pragma once

#include "./Shape.h"
#include "../common/Point.h"
#include "../common/Color.h"

class Rectangle : public Shape
{
public:
    Rectangle(Point position, float width, float height, Color fillColor, Color outlineColor)
        : Shape(fillColor, outlineColor),
          m_position(position),
          m_width(width),
          m_height(height)
    {
    }

    std::vector<Point> GetPoints() const override
    {
        return std::vector<Point>{
            {m_position.x, m_position.y},
            {m_position.x + m_width, m_position.y},
            {m_position.x + m_width, m_position.y + m_height},
            {m_position.x, m_position.y + m_height}};
    }

    void Move(float dx, float dy) override
    {
        m_position.x += dx;
        m_position.y += dy;
    }

    bool IsPointInsideShape(const Point &point) const
    {
        return point.x >= m_position.x && point.x <= m_position.x + m_width &&
               point.y >= m_position.y && point.y <= m_position.y + m_height;
    }

private:
    Point m_position;
    float m_width;
    float m_height;
};