#pragma once
#include "../Common/Point.h"
#include "../Canvas/ICanvas.h"
#include <vector>
#include <cmath>

class Valve
{
public:
    Valve()
    {
    }

    void Draw(ICanvas &canvas)
    {
        std::vector<Point> valve = {
            {-2, -70 + m_openSize * MOVE_SIZE},
            {-2, -2 + m_openSize * MOVE_SIZE},
            {-12, -2 + m_openSize * MOVE_SIZE},
            {-12, +m_openSize * MOVE_SIZE},
            {+12, +m_openSize * MOVE_SIZE},
            {+12, -2 + m_openSize * MOVE_SIZE},
            {+2, -2 + m_openSize * MOVE_SIZE},
            {+2, -70 + m_openSize * MOVE_SIZE}};

        canvas.SetColor(Color(0x00FFFFFF));
        canvas.FillPolygon(valve);
        canvas.SetColor(Color(0x000000FF));
        canvas.DrawPolygon(valve, 3);
        canvas.SetColor(Color(0xFFFFFFFF));
        canvas.FillCircle({0, -65}, 8);
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
    float m_openSize = 0;
    const float MOVE_SIZE = 5;
};