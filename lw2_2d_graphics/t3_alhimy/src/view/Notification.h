#pragma once
#include "./Canvas/ICanvas.h"
#include "./Common/Size.h"
#include "./Common/Point.h"
#include <chrono>

class Notification
{
public:
    static constexpr unsigned MILLISECOND_LIFE = 2000;
    Notification(const std::wstring &text, const Point &center)
        : m_text(text),
          m_position(center),
          m_creationTime(std::chrono::steady_clock::now())
    {
    }

    void Draw(ICanvas &canvas)
    {
        auto textSize = canvas.GetTextSize(m_text, 12);
        Size size = Size(textSize.width + 20, textSize.height + 20);
        canvas.SetColor(0x0000FFFF);
        canvas.FillRect({m_position.x - size.width / 2, m_position.y - size.height / 2}, size);
        canvas.SetColor(0xFFFFFFFF);
        canvas.DrawString(m_text, {m_position.x - textSize.width / 2, m_position.y - textSize.height / 2}, 12);
    }

    bool IsExpired() const
    {
        auto now = std::chrono::steady_clock::now();
        auto age = std::chrono::duration_cast<std::chrono::milliseconds>(
                       now - m_creationTime)
                       .count();
        return age >= MILLISECOND_LIFE;
    }

private:
    std::wstring m_text;
    Point m_position;
    std::chrono::steady_clock::time_point m_creationTime;
};