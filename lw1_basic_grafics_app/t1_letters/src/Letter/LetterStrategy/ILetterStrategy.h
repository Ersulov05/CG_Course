#pragma once

#include "../../Canvas/ICanvas.h"
#include "../../Common/Point.h"
#include "../../Common/Color.h"
#include <iostream>

class ILetterStrategy
{
public:
    virtual ~ILetterStrategy() = default;
    virtual void Draw(ICanvas &canvas, const Point &position, const unsigned size) const = 0;
    virtual std::unique_ptr<ILetterStrategy> Clone() const = 0;
};