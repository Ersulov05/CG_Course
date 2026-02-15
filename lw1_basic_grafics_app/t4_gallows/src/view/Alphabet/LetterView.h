#pragma once
#include "../Canvas/ICanvas.h"
#include "../Common/Point.h"
#include "../Common/Color.h"
#include "../../model/Alphabet.h"
#include <iostream>

const Color GREEN_COLOR = 0x00FF00FF;
const Color RED_COLOR = 0xFF0000FF;
const Color GRAY_COLOR = 0x888888FF;

class LetterView
{
public:
    using ClickCallback = std::function<void(wchar_t)>;

    LetterView(wchar_t letter, const Point &position, unsigned size, const Size &boundSize, MouseController &mouseController, ClickCallback onClick)
        : m_letter(letter),
          m_position(position),
          m_size(size),
          m_boundSize(boundSize),
          m_state(LetterState::NotUsed),
          m_mouseController(mouseController),
          m_onClick(onClick)
    {
        m_subscriptionId = m_mouseController.OnClickSubscribe(
            [this](const Point &mousePosition)
            {
                HandleClick(mousePosition);
            });
    }

    ~LetterView()
    {
        if (m_subscriptionId != 0)
        {
            m_mouseController.OnClickUnsubscribe(m_subscriptionId);
        }
    }

    void Draw(ICanvas &canvas)
    {
        // canvas.SetColor(0xFFFFFFFF);
        // unsigned padding = 2;
        // canvas.FillPolygon({
        //     {m_position.x - padding, m_position.y - padding},
        //     {m_position.x + m_boundSize.width + padding, m_position.y - padding},
        //     {m_position.x + m_boundSize.width + padding, m_position.y + m_boundSize.height + padding},
        //     {m_position.x - padding, m_position.y + m_boundSize.height + padding},
        // });
        switch (m_state)
        {
        case LetterState::Correct:
            canvas.SetColor(GREEN_COLOR);
            break;
        case LetterState::Wrong:
            canvas.SetColor(RED_COLOR);
            break;
        default:
            canvas.SetColor(GRAY_COLOR);
            break;
        }

        canvas.DrawChar(m_letter, m_position, m_size);
    }

    void SetState(LetterState state)
    {
        m_state = state;
    }

private:
    wchar_t m_letter;
    Point m_position;
    unsigned m_size;
    Size m_boundSize;
    unsigned m_subscriptionId;
    MouseController &m_mouseController;
    LetterState m_state;
    ClickCallback m_onClick;

    void HandleClick(const Point &mousePosition)
    {
        unsigned padding = 2;
        if (mousePosition.x >= m_position.x - padding &&
            mousePosition.x <= m_position.x + m_boundSize.width + padding &&
            mousePosition.y >= m_position.y - padding &&
            mousePosition.y <= m_position.y + m_boundSize.height + padding)
        {
            if (m_onClick)
            {
                m_onClick(m_letter);
            }
        }
    }
};