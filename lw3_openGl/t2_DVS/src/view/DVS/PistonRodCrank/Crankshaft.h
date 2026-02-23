#pragma once
#include "../../Canvas/ICanvas.h"
#include "../../Common/Point.h"

class Crankshaft
{
public:
    Crankshaft(const Point &position, float radius, float rotation, float scale)
        : m_position(position),
          m_rotation(rotation),
          m_radius(radius),
          m_scale(scale)
    {
    }

    void Draw(ICanvas &canvas)
    {
        auto x = m_position.x;
        auto y = m_position.y;

        std::vector<Point> baseCrankshaft = {
            {x - m_radius, y},
            {x - m_radius - 20, y + 40},
            {x - m_radius - 10, y + 50},
            {x + m_radius + 10, y + 50},
            {x + m_radius + 20, y + 40},
            {x + m_radius, y}};

        std::vector<Point> crankshaft = TransformPoints(baseCrankshaft, m_position, m_rotation, m_scale);
        canvas.SetColor(Color(0xFFFFD700));
        canvas.FillPolygon(crankshaft);
        canvas.SetColor(Color(0xFF000000));
        canvas.DrawPolygon(crankshaft, 3, false);

        canvas.SetColor(Color(0xFF000000));
        canvas.FillCircle(m_position, (m_radius + 1) * m_scale);
        canvas.SetColor(Color(0xFFFFFFFF));
        canvas.FillCircle(m_position, m_radius * m_scale);
    }

    void SetRotation(float rotation)
    {
        m_rotation = rotation;
    }

private:
    Point m_position;
    float m_rotation;
    float m_scale;
    float m_radius;
};