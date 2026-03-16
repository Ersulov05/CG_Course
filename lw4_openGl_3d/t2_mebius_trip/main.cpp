#include <iostream>
#include "./src/view/Canvas/Canvas3D.h"
#include "./src/view/Canvas/ICanvas3D.h"
#include "./src/view/MebiusTrip.h"

int main()
{
    Canvas3D canvas(800, 600);
    MebiusTrip ms;

    canvas.Run(
        [&ms](ICanvas3D &canvas, float deltatime)
        {
            ms.Draw(canvas);
        });

    return 0;
}