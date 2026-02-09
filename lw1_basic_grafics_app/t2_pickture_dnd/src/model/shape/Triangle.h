#pragma once

#include "./Shape.h"
#include "../common/Point.h"
#include "../common/Color.h"

class Triangle : public Shape
{
public:
    Triangle(Point firstPoint, Point secondPoint, Point thirdPoint, Color fillColor, Color outlineColor)
        : Shape(fillColor, outlineColor),
          m_firstPoint(firstPoint),
          m_secondPoint(secondPoint),
          m_thirdPoint(thirdPoint)
    {
    }

    std::vector<Point> GetPoints() const override
    {
        return std::vector<Point>{
            m_firstPoint,
            m_secondPoint,
            m_thirdPoint};
    }

    void Move(float dx, float dy) override
    {
        m_firstPoint.x += dx;
        m_firstPoint.y += dy;
        m_secondPoint.x += dx;
        m_secondPoint.y += dy;
        m_thirdPoint.x += dx;
        m_thirdPoint.y += dy;
    }

    bool IsPointInsideShape(const Point &point) const override
    {
        auto a = GetPointSide(m_firstPoint, m_secondPoint, point);
        if (a == 0)
        {
            return true;
        }
        auto b = GetPointSide(m_secondPoint, m_thirdPoint, point);
        if (b == 0)
        {
            return true;
        }
        auto c = GetPointSide(m_thirdPoint, m_firstPoint, point);
        if (c == 0)
        {
            return true;
        }
        if (a < 0 && b < 0 && c < 0 || a > 0 && b > 0 && c > 0)
        {
            return true;
        }
        return false;
    }

private:
    Point m_firstPoint;
    Point m_secondPoint;
    Point m_thirdPoint;

    int GetPointSide(const Point &startPoint, const Point &endPoint, const Point &point) const
    {
        return (endPoint.x - startPoint.x) * (point.y - startPoint.y) -
               (endPoint.y - startPoint.y) * (point.x - startPoint.x);
    }
};