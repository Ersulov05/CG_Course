#pragma once
#include "./Common/Point.h"
#include "../model/Element.h"
#include "./Canvas/ICanvas.h"
#include "./Canvas/Drawable.h"
#include "./Constants.h"

class ElementView : public Drawable
{
public:
    using OnPressCallback = std::function<void(const Point &, const Element &)>;

    ElementView(ICanvas &canvas, const Element &element, const Point &position, OnPressCallback onPress)
        : m_canvas(canvas),
          m_element(element),
          m_position(position),
          m_onPress(onPress)
    {
        auto &mouseController = canvas.GetMouseController();
        mouseController.OnPressSubscribe(
            [this](const Point &mousePosition)
            {
                PressHandle(mousePosition);
            },
            [this]()
            { return GetZIndex(); });
    }

    void Draw(unsigned &zIndex) override
    {
        Drawable::Draw(zIndex);
        m_canvas.SetColor(0xFFFFFFFF);
        m_canvas.DrawRect(m_position, Constants::ELEMENT_SIZE);
        m_canvas.DrawImage(m_element.GetImagePath(), m_position, Constants::ELEMENT_SIZE);
        auto textSize = m_canvas.GetTextSize(m_element.GetName(), 12);
        m_canvas.DrawString(m_element.GetName(), {m_position.x + Constants::ELEMENT_SIZE.width / 2 - textSize.width / 2, m_position.y + Constants::ELEMENT_SIZE.height}, 12);
    }

    void SetPosition(const Point &position)
    {
        m_position = position;
    }

    Element GetElement() const
    {
        return m_element;
    }

private:
    ICanvas &m_canvas;
    Element m_element;
    Point m_position;
    OnPressCallback m_onPress;

    void PressHandle(const Point &mousePosition)
    {
        if (ContainsPoint(mousePosition))
        {
            m_canvas.GetMouseController().StopPropagation();
            m_onPress(m_position, m_element);
        }
    }

    bool ContainsPoint(const Point &point) const
    {
        return point.x >= m_position.x &&
               point.x <= m_position.x + Constants::ELEMENT_SIZE.width &&
               point.y >= m_position.y &&
               point.y <= m_position.y + Constants::ELEMENT_SIZE.height;
    }
};