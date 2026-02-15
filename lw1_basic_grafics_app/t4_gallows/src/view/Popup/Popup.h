#pragma once
#include "../Canvas/ICanvas.h"
#include "../Common/Point.h"
#include "../Common/Size.h"
#include "../Common/Size.h"
#include "../../model/Gallows.h"

using ClickCallback = std::function<void()>;

struct ButtonData
{
    Point position;
    Size size;
    Color color;
    std::wstring text;
    ClickCallback onClick;

    ButtonData(const std::wstring &text, const Point &position, const Size &size, Color color, ClickCallback onClick)
        : text(text), position(position), size(size), color(color), onClick(onClick) {}
};

class Popup
{
public:
    const Size POPUP_SIZE = Size(290, 100);
    const Point POPUP_POSITION = Point(355, 250);

    Popup(
        ICanvas &canvas,
        ClickCallback resetOnClick)
        : m_canvas(canvas),
          m_resetButton(ButtonData(L"ЗАНОВО", {20, 50}, {100, 20}, 0xFF0000FF, resetOnClick)),
          m_exitButton(ButtonData(
              L"ВЫЙТИ", {170, 50}, {100, 20}, 0xFF0000FF,
              [&canvas]()
              {
                  canvas.Exit();
              }))
    {
    }

    void Draw()
    {
        if (!m_isOpened)
        {
            return;
        }
        m_canvas.SetColor(0xFFFFFFFF);
        m_canvas.FillPolygon({{POPUP_POSITION.x, POPUP_POSITION.y},
                              {POPUP_POSITION.x + POPUP_SIZE.width, POPUP_POSITION.y},
                              {POPUP_POSITION.x + POPUP_SIZE.width, POPUP_POSITION.y + POPUP_SIZE.height},
                              {POPUP_POSITION.x, POPUP_POSITION.y + POPUP_SIZE.height}});
        auto textSize = m_canvas.GetTextSize(m_text, 24);
        m_canvas.SetColor(0x888888FF);
        m_canvas.DrawString(m_text, {POPUP_POSITION.x + POPUP_SIZE.width / 2 - textSize.width / 2, POPUP_POSITION.y}, 24);
        DrawButton(m_resetButton);
        DrawButton(m_exitButton);
    }

    void Open()
    {
        if (m_isOpened)
        {
            return;
        }

        m_isOpened = true;
        m_subscriptionId = m_canvas.GetMouseController().OnClickSubscribe(
            [this](const Point &mousePosition)
            {
                HandleClickButton(mousePosition, this->m_resetButton);
                HandleClickButton(mousePosition, this->m_exitButton);
            });
    }

    void Close()
    {
        if (!m_isOpened)
        {
            return;
        }

        m_canvas.GetMouseController().OnClickUnsubscribe(m_subscriptionId);
        m_isOpened = false;
    }

    void SetText(const std::wstring &text)
    {
        m_text = text;
    }

    bool IsOpened()
    {
        return m_isOpened;
    }

private:
    bool m_isOpened = false;
    unsigned m_subscriptionId = 0;
    ICanvas &m_canvas;
    ButtonData m_resetButton;
    ButtonData m_exitButton;
    std::wstring m_text;

    void HandleClickButton(const Point &mousePosition, const ButtonData &button)
    {
        if (mousePosition.x >= button.position.x + POPUP_POSITION.x &&
            mousePosition.x <= button.position.x + POPUP_POSITION.x + button.size.width &&
            mousePosition.y >= button.position.y + POPUP_POSITION.y &&
            mousePosition.y <= button.position.y + POPUP_POSITION.y + button.size.height)
        {
            button.onClick();
            Close();
        }
    }

    void DrawButton(const ButtonData &button)
    {
        m_canvas.SetColor(button.color);
        m_canvas.FillPolygon({{POPUP_POSITION.x + button.position.x, POPUP_POSITION.y + button.position.y},
                              {POPUP_POSITION.x + button.position.x + button.size.width, POPUP_POSITION.y + button.position.y},
                              {POPUP_POSITION.x + button.position.x + button.size.width, POPUP_POSITION.y + button.position.y + button.size.height},
                              {POPUP_POSITION.x + button.position.x, POPUP_POSITION.y + button.position.y + button.size.height}});
        auto boundSize = m_canvas.GetTextSize(button.text, 18);
        m_canvas.SetColor(0xFFFFFFFF);
        m_canvas.DrawString(
            button.text,
            {POPUP_POSITION.x + button.position.x + button.size.width / 2 - boundSize.width / 2,
             POPUP_POSITION.y + button.position.y + button.size.height / 2 - boundSize.height / 2 - 2},
            18);
    }
};