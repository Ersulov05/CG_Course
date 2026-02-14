#pragma once
#include "./IGallowsViewStrategy.h"

class GallowsPerformanceViewStrategy : public IGallowsViewStrategy
{
public:
    void Draw(ICanvas &canvas, unsigned attemptCount) override
    {
        canvas.FillPolygon({{100, 100},
                            {200, 100},
                            {200, 200}});
    }

private:
};