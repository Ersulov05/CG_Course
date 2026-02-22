#pragma once
#include "./Common/Point.h"
#include "./Common/Size.h"
#include "./Canvas/ICanvas.h"

class Draggable
{
public:
    Draggable(ICanvas &canvas, const Point &position, const Size &size)
        : m_canvas(canvas),
          m_position(position),
          m_size(size)
    {
        auto &mouseController = m_canvas.GetMouseController();

        m_pressSubscriptionId = mouseController.OnPressSubscribe(
            [this](const Point &mousePosition)
            {
                HandlePress(mousePosition);
            });

        m_releaseSubscriptionId = mouseController.OnReleaseSubscribe(
            [this](const Point &mousePosition)
            {
                HandleRelease(mousePosition);
            });
        m_moveSubscriptionId = mouseController.OnMoveSubscribe(
            [this](const Point &mousePosition)
            { HandleMove(mousePosition); });
    }

    ~Draggable()
    {
        m_canvas.GetMouseController().OnPressUnsubscribe(m_pressSubscriptionId);
        m_canvas.GetMouseController().OnReleaseUnsubscribe(m_releaseSubscriptionId);
        m_canvas.GetMouseController().OnMoveUnsubscribe(m_moveSubscriptionId);
    }

    Point GetPosition()
    {
        return m_position;
    }

private:
    bool m_isPressed = false;
    ICanvas &m_canvas;
    Point m_mouseOffset;
    Point m_position;
    Size m_size;
    unsigned m_pressSubscriptionId;
    unsigned m_releaseSubscriptionId;
    unsigned m_moveSubscriptionId;

    void HandlePress(const Point &mousePosition)
    {
        if (m_isPressed || !ContainsPoint(mousePosition))
        {
            return;
        }
        m_canvas.GetMouseController().StopPropagation();
        m_isPressed = true;
        m_mouseOffset = mousePosition - m_position;
    }

    void HandleRelease(const Point &mousePosition)
    {
        if (!m_isPressed)
        {
            return;
        }
        m_canvas.GetMouseController().StopPropagation();
        m_isPressed = false;
    }

    void HandleMove(const Point &mousePosition)
    {
        if (!m_isPressed)
        {
            return;
        }

        m_position = mousePosition - m_mouseOffset;
    }

    bool ContainsPoint(const Point &point) const
    {
        return point.x >= m_position.x &&
               point.x <= m_position.x + m_size.width &&
               point.y >= m_position.y &&
               point.y <= m_position.y + m_size.height;
    }
};