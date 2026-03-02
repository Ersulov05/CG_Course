#pragma once
#include "../Canvas/ICanvas.h"
#include "../../model/Game.h"
#include "../../common/Point.h"
#include "../../common/Size.h"

struct ButtonData
{
    Point position;
    Size size;
    Color color;

    ButtonData(const Point &position, const Size &size, const Color &color) : position(position), size(size), color(color) {}
};

class EndGamePopup
{
public:
    const Size SIZE = {300, 160};
    EndGamePopup(ICanvas &canvas, Game &game)
        : m_canvas(canvas),
          m_game(game)
    {
    }

    void Draw()
    {
        if (!m_isOpen)
            return;

        m_canvas.PushMatrix();
        m_canvas.Translate(GetPopupPosition());
        m_canvas.SetColor(0xFFFFFFFF);
        m_canvas.FillRect({-SIZE.width / 2, -SIZE.height / 2}, SIZE);
        m_canvas.SetColor(0x000000FF);
        auto textSize = m_canvas.GetTextBoundSize("End game", 30);
        m_canvas.DrawText("End game", {-textSize.width / 2, -25}, 30);
        std::string scoreText = "Your score: " + std::to_string(m_game.GetScore());
        textSize = m_canvas.GetTextBoundSize(scoreText, 30);
        m_canvas.DrawText(scoreText, {-textSize.width / 2, textSize.height / 2}, 30);
        DrawResetButton();
        m_canvas.PopMatrix();
    }

    void Open()
    {
        if (m_isOpen)
            return;
        m_isOpen = true;
        m_subscriptionId = m_canvas.GetMouseController().OnPressSubscribe(
            [this](const Point &mousePos)
            {
                HandleClickResetButton(mousePos);
            });
    }

    void Close()
    {
        if (!m_isOpen)
            return;
        m_isOpen = false;

        m_canvas.GetMouseController().OnPressUnsubscribe(m_subscriptionId);
    }

private:
    ICanvas &m_canvas;
    Game &m_game;
    bool m_isOpen = false;
    unsigned m_subscriptionId;
    ButtonData m_resetButton = ButtonData({-60, 25}, {120, 28}, 0x00AAAAFF);

    void DrawResetButton()
    {
        m_canvas.PushMatrix();
        m_canvas.Translate(m_resetButton.position);
        m_canvas.SetColor(m_resetButton.color);
        m_canvas.FillRect({0, 0}, m_resetButton.size);
        m_canvas.SetColor(0x000000FF);
        auto textSize = m_canvas.GetTextBoundSize("Reset", 30);
        m_canvas.DrawText("Reset", {m_resetButton.size.width / 2 - textSize.width / 2, m_resetButton.size.height / 2 + textSize.height / 2 - 1}, 30);
        m_canvas.PopMatrix();
    }

    Point GetPopupPosition() const
    {
        return {m_canvas.GetWidth() / 2, m_canvas.GetHeight() / 2};
    }

    void HandleClickResetButton(const Point &mousePosition)
    {
        auto popupPosition = GetPopupPosition();
        if (mousePosition.x >= m_resetButton.position.x + popupPosition.x &&
            mousePosition.x <= m_resetButton.position.x + popupPosition.x + m_resetButton.size.width &&
            mousePosition.y >= m_resetButton.position.y + popupPosition.y &&
            mousePosition.y <= m_resetButton.position.y + popupPosition.y + m_resetButton.size.height)
        {
            m_game.Reset();
            Close();
        }
    }
};