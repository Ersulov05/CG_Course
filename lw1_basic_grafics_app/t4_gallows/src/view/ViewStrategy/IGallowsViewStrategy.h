#pragma once
#include "../Canvas/ICanvas.h"

class IGallowsViewStrategy
{
public:
    virtual void Draw(ICanvas &canvas, unsigned attemptCount) = 0;
};