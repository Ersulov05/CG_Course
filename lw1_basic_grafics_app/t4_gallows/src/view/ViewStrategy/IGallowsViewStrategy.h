#pragma once
#include "../Canvas/ICanvas.h"

enum class GallowsViewType
{
    Graphics,
    Text
};

class IGallowsViewStrategy
{
public:
    virtual void Draw(ICanvas &canvas, unsigned attemptCount) = 0;
    virtual GallowsViewType GetType() = 0;
};