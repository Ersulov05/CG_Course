#pragma once
#include "../../model/common/Point.h"

// struct Point
// {
//     float x;
//     float y;

//     Point() : x(0), y(0) {}
//     Point(float x, float y) : x(x), y(y) {}
// };

class DragState
{
public:
    void StartDrag(Point startDragPosition)
    {
        m_isDragging = true;
        m_draggingStartPosition = startDragPosition;
        m_currentPosition = startDragPosition;
    }

    void ResetDrag()
    {
        m_isDragging = false;
    }

    bool IsDragging()
    {
        return m_isDragging;
    }

    void Move(Point position)
    {
        if (m_isDragging)
        {
            m_currentPosition = position;
        }
    }

    Point GetDragging()
    {
        if (m_isDragging)
        {
            return Point(
                m_currentPosition.x - m_draggingStartPosition.x,
                m_currentPosition.y - m_draggingStartPosition.y);
        }
        return Point();
    }

private:
    bool m_isDragging;
    Point m_draggingStartPosition;
    Point m_currentPosition;
};