#pragma once
#include "../Canvas/ICanvas.h"
#include "../Common/Point.h"

class SparkPlug
{
public:
    SparkPlug()
    {
    }

    void Draw(ICanvas &canvas)
    {
        canvas.SetColor(Color(0xFFFFFFFF));
        canvas.FillRect({-5, -50}, {10, 30});
        canvas.SetColor(Color(0x000000FF));
        canvas.FillRect({-2, -55}, {4, 5});
        canvas.SetColor(Color(0x4D4D4DFF));
        canvas.FillRect({-7, -20}, {14, 4});
        canvas.FillRect({-10, -16}, {20, 6});
        canvas.FillRect({-7, -10}, {14, 2});
        canvas.FillRect({-10, -8}, {20, 6});

        canvas.FillRect({-6, -2}, {12, 14});
        canvas.SetColor(Color(0x000000FF));
        canvas.FillRect({-2, +12}, {4, 4});
    }
};