#pragma once

#include "../../Common/Point.h"
#include "./ILetterStrategy.h"
#include <vector>
#include <iostream>

class LetterAStrategy : public ILetterStrategy
{
public:
    void Draw(ICanvas &canvas, const Point &position, const unsigned size) const override
    {
        float width = size;
        float height = size / 4 * 5;

        canvas.FillPolygon({{position.x, position.y + height},
                            {position.x + width / 2, position.y},
                            {position.x + width / 2, position.y + height / 4},
                            {position.x + width / 5, position.y + height}});

        canvas.FillPolygon({{position.x + width, position.y + height},
                            {position.x + width / 2, position.y},
                            {position.x + width / 2, position.y + height / 4},
                            {position.x + width * 4 / 5, position.y + height}});

        canvas.FillPolygon({{position.x + 0.25f * width, position.y + 0.55f * height},
                            {position.x + 0.25f * width, position.y + 0.7f * height},
                            {position.x + 0.75f * width, position.y + 0.7f * height},
                            {position.x + 0.75f * width, position.y + 0.55f * height}});
    };

    std::unique_ptr<ILetterStrategy> Clone() const override
    {
        return std::make_unique<LetterAStrategy>(*this);
    }
};