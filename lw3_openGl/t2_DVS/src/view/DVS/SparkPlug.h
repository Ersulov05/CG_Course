#pragma once
#include "../Canvas/ICanvas.h"
#include "../Common/Point.h"

class SparkPlug
{
public:
    SparkPlug(const Point &position, float scale)
        : m_position(position),
          m_scale(scale)
    {
    }

    void Draw(ICanvas &canvas)
    {
        auto x = m_position.x;
        auto y = m_position.y;

        canvas.SetColor(Color(0xFFFFFFFF));
        canvas.FillRect({m_position.x - 5 * m_scale, m_position.y - 50 * m_scale}, {10 * m_scale, 30 * m_scale});
        canvas.SetColor(Color(0xFF000000));
        canvas.FillRect({m_position.x - 2 * m_scale, m_position.y - 55 * m_scale}, {4 * m_scale, 5 * m_scale});
        canvas.SetColor(Color(0xFF4D4D4D));
        canvas.FillRect({m_position.x - 7 * m_scale, m_position.y - 20 * m_scale}, {14 * m_scale, 4 * m_scale});
        canvas.FillRect({m_position.x - 10 * m_scale, m_position.y - 16 * m_scale}, {20 * m_scale, 6 * m_scale});
        canvas.FillRect({m_position.x - 7 * m_scale, m_position.y - 10 * m_scale}, {14 * m_scale, 2 * m_scale});
        canvas.FillRect({m_position.x - 10 * m_scale, m_position.y - 8 * m_scale}, {20 * m_scale, 6 * m_scale});

        canvas.FillRect({m_position.x - 6 * m_scale, m_position.y - 2 * m_scale}, {12 * m_scale, 14 * m_scale});
        canvas.SetColor(Color(0xFF000000));
        canvas.FillRect({m_position.x - 2 * m_scale, m_position.y + 12 * m_scale}, {4 * m_scale, 4 * m_scale});
    }

private:
    Point m_position;
    float m_scale;
};