#pragma once

#include "../../Common/Point.h"
#include "./ILetterStrategy.h"
#include <vector>
#include <iostream>

class LetterEStrategy : public ILetterStrategy
{
public:
    void Draw(ICanvas &canvas, const Point &position, const unsigned size) const override
    {
        float width = size;
        float height = size / 4 * 5;

        canvas.FillPolygon({{position.x, position.y},
                            {position.x + width / 4, position.y},
                            {position.x + width / 4, position.y + height},
                            {position.x, position.y + height}});

        canvas.FillPolygon({{position.x, position.y},
                            {position.x + width, position.y},
                            {position.x + width, position.y + 0.2f * height},
                            {position.x, position.y + 0.2f * height}});

        canvas.FillPolygon({{position.x, position.y + 0.4f * height},
                            {position.x + width, position.y + 0.4f * height},
                            {position.x + width, position.y + 0.6f * height},
                            {position.x, position.y + 0.6f * height}});

        canvas.FillPolygon({{position.x, position.y + 0.8f * height},
                            {position.x + width, position.y + 0.8f * height},
                            {position.x + width, position.y + height},
                            {position.x, position.y + height}});
    };

    std::unique_ptr<ILetterStrategy> Clone() const override
    {
        return std::make_unique<LetterEStrategy>(*this);
    }
};