#pragma once

#include "../Common/Point.h"
#include "../Common/Color.h"
#include "./LetterStrategy/ILetterStrategy.h"
#include "./MovementStrategy/IMovementStrategy.h"
#include <cassert>
#include <memory>
#include <string>

class Letter
{
public:
    Letter(std::unique_ptr<ILetterStrategy> letterStrategy,
           std::unique_ptr<IMovementStrategy> movementStrategy,
           Color color = 0xffffffff,
           Point position = {0, 0},
           unsigned size = 100)
        : m_letterStrategy(std::move(letterStrategy)),
          m_movementStrategy(std::move(movementStrategy)),
          m_color(color),
          m_position(position),
          m_size(size)
    {
        assert(m_letterStrategy);
        assert(m_movementStrategy);
    };

    void Move()
    {
        m_movementStrategy->Move();
    }

    void Draw(ICanvas &canvas) const
    {
        canvas.SetColor(m_color);
        auto movementPosition = m_movementStrategy->GetPosition();
        Point position = {m_position.x + movementPosition.x, m_position.y + movementPosition.y};
        m_letterStrategy->Draw(canvas, position, m_size);
    }

private:
    unsigned m_size;
    Color m_color;
    Point m_position;
    std::unique_ptr<ILetterStrategy> m_letterStrategy;
    std::unique_ptr<IMovementStrategy> m_movementStrategy;
};