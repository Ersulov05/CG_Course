#pragma once
#include "../Common/Point.h"
#include "../Canvas/ICanvas.h"
#include <vector>
#include <cmath>

class Valve
{
public:
    Valve(const Point &position, float rotation, float scale)
        : m_position(position),
          m_rotation(rotation),
          m_scale(scale)
    {
    }

    void Draw(ICanvas &canvas)
    {
        auto x = m_position.x;
        auto y = m_position.y;

        std::vector<Point> baseValve = {
            {x - 2, y - 70 + m_openSize * MOVE_SIZE},
            {x - 2, y - 2 + m_openSize * MOVE_SIZE},
            {x - 12, y - 2 + m_openSize * MOVE_SIZE},
            {x - 12, y + m_openSize * MOVE_SIZE},
            {x + 12, y + m_openSize * MOVE_SIZE},
            {x + 12, y - 2 + m_openSize * MOVE_SIZE},
            {x + 2, y - 2 + m_openSize * MOVE_SIZE},
            {x + 2, y - 70 + m_openSize * MOVE_SIZE}};

        std::vector<Point> valve;
        Point center(x, y);

        for (const auto &p : baseValve)
        {
            valve.push_back(TransformPoint(p, center, m_rotation, m_scale));
        }
        Point circleCenter = TransformPoint(Point(x, y - 65), center, m_rotation, m_scale);

        canvas.SetColor(Color(0xFF00FFFF));
        canvas.FillPolygon(valve);
        canvas.SetColor(Color(0xFF000000));
        canvas.DrawPolygon(valve, 3);
        canvas.SetColor(Color(0xFFFFFFFF));
        canvas.FillCircle(circleCenter, 8 * m_scale);
        canvas.SetColor(Color(0xFFFFFFFF));
    }

    void SetOpen(float openSize)
    {
        m_openSize = openSize;
        if (m_openSize > 1)
        {
            m_openSize = 1;
        }
        if (m_openSize < 0)
        {
            m_openSize = 0;
        }
    }

private:
    Point m_position;
    float m_rotation;
    float m_openSize = 0;
    const float MOVE_SIZE = 5;
    float m_scale;
};