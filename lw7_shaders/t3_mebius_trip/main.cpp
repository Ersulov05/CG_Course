#include <iostream>
#include "./src/view/Canvas/Canvas3D.h"
#include "./src/view/Canvas/ICanvas3D.h"
#include "./src/view/MebiusTrip.h"
#include "./src/view/MorphingSurface.h"

int main()
{
    Canvas3D canvas(800, 600);
    MorphingSurface ms(40);

    canvas.Run(
        [&ms](ICanvas3D &canvas, float deltatime)
        {
            ms.Draw(canvas, 0.0);
        });

    return 0;
}