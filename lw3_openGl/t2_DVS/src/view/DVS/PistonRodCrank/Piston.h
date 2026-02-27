#pragma once
#include "../../Canvas/ICanvas.h"
#include "../../Common/Point.h"

class Piston
{
public:
    Piston()
    {
    }

    void Draw(ICanvas &canvas)
    {
        canvas.SetColor(Color(0x8F8F8FFF));
        canvas.FillRect({-50, -25}, {100, 50});
        canvas.SetColor(Color(0x000000FF));
        canvas.DrawRect({-50, -25}, {100, 50}, 3);
        canvas.SetColor(Color(0xFFFFFFFF));
        canvas.FillCircle({0, 0}, 10);
    }
};