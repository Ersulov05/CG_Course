#pragma once
#include "../Canvas/ICanvas.h"
#include "../Common/Color.h"
#include "../Common/Point.h"
#include "../Common/Size.h"

class Button
{
public:
    using OnClickCallback = std::function<void()>;

    Button(
        ICanvas &canvas,
        Point position,
        Size size,
        Color color,
        Color textColor,
        OnClickCallback onClick)
        : m_canvas(canvas),
          m_position(position),
          m_size(size),
          m_color(color),
          m_textColor(textColor),
          m_onClick(onClick)
    {
        auto &mouseController = m_canvas.GetMouseController();
        m_subscriptionId = mouseController.OnClickSubscribe(
            [this](const Point &mousePos)
            { HandleClick(mousePos); });
    }

    ~Button()
    {
        m_canvas.GetMouseController().OnClickUnsubscribe(m_subscriptionId);
    }

    void Draw(const std::wstring text)
    {
        m_canvas.SetColor(m_color);
        m_canvas.DrawRect(m_position, m_size);
        m_canvas.SetColor(m_textColor);
        const unsigned size = 14;
        auto boundSize = m_canvas.GetTextSize(text, size);
        m_canvas.DrawString(text, {m_position.x + m_size.width / 2 - boundSize.width / 2, m_position.y + 2}, size);
    }

private:
    ICanvas &m_canvas;
    Point m_position = {0, 0};
    Size m_size = {0, 0};
    Color m_color;
    Color m_textColor;
    OnClickCallback m_onClick;
    unsigned m_subscriptionId = 0;

    void HandleClick(const Point &mousePos)
    {
        if (mousePos.x >= m_position.x &&
            mousePos.x <= m_position.x + m_size.width &&
            mousePos.y >= m_position.y &&
            mousePos.y <= m_position.y + m_size.height)
        {
            if (m_onClick)
            {
                m_onClick();
            }
        }
    }
};