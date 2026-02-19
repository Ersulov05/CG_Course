#pragma once
#include "./Common/Point.h"
#include "./Common/Size.h"
#include "../model/Element.h"
#include "./Canvas/ICanvas.h"
#include "./Canvas/Drawable.h"
#include <functional>
#include "./Constants.h"

class DragElementView : public Drawable
{
public:
    using OnReleaseCallback = std::function<void(DragElementView *)>;
    using OnPressCallback = std::function<void(DragElementView *)>;

    DragElementView(ICanvas &canvas, const Element &element, const Point &position, OnReleaseCallback onRelease, OnPressCallback onPress, bool isPressed = false)
        : m_canvas(canvas),
          m_element(element),
          m_position(position),
          m_mouseOffset(canvas.GetMouseController().GetMousePosition() - position),
          m_onRelease(onRelease),
          m_onPress(onPress),
          m_isPressed(isPressed)
    {
        auto &mouseController = m_canvas.GetMouseController();

        m_pressSubscriptionId = mouseController.OnPressSubscribe(
            [this](const Point &mousePosition)
            { HandlePress(mousePosition); },
            [this]()
            { return GetZIndex(); });

        m_releaseSubscriptionId = mouseController.OnReleaseSubscribe(
            [this](const Point &mousePosition)
            {
                HandleRelease(mousePosition);
            },
            [this]()
            { return GetZIndex(); });
        m_moveSubscriptionId = mouseController.OnMoveSubscribe(
            [this](const Point &mousePosition)
            { HandleMove(mousePosition); },
            [this]()
            { return GetZIndex(); });
    }

    ~DragElementView()
    {
        m_canvas.GetMouseController().OnPressUnsubscribe(m_pressSubscriptionId);
        m_canvas.GetMouseController().OnReleaseUnsubscribe(m_releaseSubscriptionId);
        m_canvas.GetMouseController().OnMoveUnsubscribe(m_moveSubscriptionId);
    }

    void Draw(unsigned &zIndex) override
    {
        Drawable::Draw(zIndex);
        m_canvas.SetColor(0xFFFFFFFF);
        m_canvas.DrawRect(m_position, Constants::ELEMENT_SIZE);
        m_canvas.DrawImage(m_element.GetImagePath(), m_position, Constants::ELEMENT_SIZE);
        m_canvas.SetColor(0xFFFFFFFF);
        auto textSize = m_canvas.GetTextSize(m_element.GetName(), 12);
        m_canvas.DrawString(m_element.GetName(), {m_position.x + Constants::ELEMENT_SIZE.width / 2 - textSize.width / 2, m_position.y + Constants::ELEMENT_SIZE.height + 3}, 12);
    }

    void SetPosition(const Point &position)
    {
        m_position = position;
    }

    Point GetPosition() const
    {
        return m_position;
    }

    Size GetSize() const
    {
        return Constants::ELEMENT_SIZE;
    }

    Element GetElement() const
    {
        return m_element;
    }

private:
    ICanvas &m_canvas;
    bool m_isPressed = true;
    Point m_mouseOffset;
    Element m_element;
    Point m_position;
    unsigned m_pressSubscriptionId;
    unsigned m_releaseSubscriptionId;
    unsigned m_moveSubscriptionId;
    unsigned m_id;
    OnReleaseCallback m_onRelease;
    OnPressCallback m_onPress;

    void HandlePress(const Point &mousePosition)
    {
        if (m_isPressed || !ContainsPoint(mousePosition))
        {
            return;
        }
        m_onPress(this);
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
        m_onRelease(this);
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
               point.x <= m_position.x + Constants::ELEMENT_SIZE.width &&
               point.y >= m_position.y &&
               point.y <= m_position.y + Constants::ELEMENT_SIZE.height;
    }
};