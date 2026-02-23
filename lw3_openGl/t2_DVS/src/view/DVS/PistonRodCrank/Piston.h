#pragma once
#include "../../Canvas/ICanvas.h"
#include "../../Common/Point.h"

class Piston
{
public:
    Piston(const Point &position, float scale)
        : m_position(position),
          m_scale(scale)
    {
    }

    void Draw(ICanvas &canvas)
    {
        canvas.SetColor(Color(0xFF8F8F8F));
        canvas.FillRect({m_position.x - 50 * m_scale, m_position.y - 25 * m_scale}, {100 * m_scale, 50 * m_scale});
        canvas.SetColor(Color(0xFF000000));
        canvas.DrawRect({m_position.x - 50 * m_scale, m_position.y - 25 * m_scale}, {100 * m_scale, 50 * m_scale}, 3);
        canvas.SetColor(Color(0xFFFFFFFF));
        canvas.FillCircle(m_position, 10);
    }

    void SetPosition(const Point &position)
    {
        m_position = position;
    }

private:
    Point m_position;
    float m_scale;
};