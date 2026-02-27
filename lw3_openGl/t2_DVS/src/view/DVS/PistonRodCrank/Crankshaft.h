#pragma once
#include "../../Canvas/ICanvas.h"
#include "../../Common/Point.h"

class Crankshaft
{
public:
    Crankshaft(float radius)
        : m_radius(radius)
    {
    }

    void Draw(ICanvas &canvas)
    {
        std::vector<Point> crankshaft = {
            {-m_radius, 0},
            {-m_radius - 20, 40},
            {-m_radius - 10, 50},
            {m_radius + 10, 50},
            {m_radius + 20, 40},
            {m_radius, 0}};

        canvas.SetColor(Color(0xFFD700FF));
        canvas.FillPolygon(crankshaft);
        canvas.SetColor(Color(0x000000FF));
        canvas.DrawPolygon(crankshaft, 3, false);

        canvas.SetColor(Color(0x000000FF));
        canvas.FillCircle({0, 0}, (m_radius + 1));
        canvas.SetColor(Color(0xFFFFFFFF));
        canvas.FillCircle({0, 0}, m_radius);
    }

private:
    float m_radius;
};