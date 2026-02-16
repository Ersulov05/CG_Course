#pragma once
#include "./Common/Point.h"
#include "./Common/Size.h"

namespace Constants
{
    const unsigned WIDTH = 1000;
    const unsigned HEIGHT = 600;
    const Point EXPERIMENT_AREA_POSITION = {WIDTH / 2, 1};
    const Size EXPERIMENT_AREA_SIZE = {WIDTH / 2 - 2, HEIGHT - 2};
    const Size ELEMENT_SIZE = {50, 50};
    const int GRID_COLS = 5;
    const int GRID_ROWS = 3;
    const int GRID_SPACING = 20;
}