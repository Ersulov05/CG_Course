#pragma once
#include "./Common/Point.h"
#include "./Common/Size.h"
#include "./Canvas/ICanvas.h"
#include "./Draggable.h"

const float SIZE = 10;

class PointView : public Draggable
{
public:
    PointView(ICanvas &canvas, const Point &position)
        : Draggable(canvas, position - SIZE / 2, {SIZE, SIZE}),
          m_canvas(canvas)
    {
    }

    void Draw()
    {
        auto pos = Draggable::GetPosition();
        m_canvas.DrawRect(pos, {SIZE, SIZE});
    }

    Point GetPosition()
    {
        return Draggable::GetPosition();
    }

    Point GetCenterPosition()
    {
        return GetPosition() + SIZE / 2;
    }

private:
    ICanvas &m_canvas;
};