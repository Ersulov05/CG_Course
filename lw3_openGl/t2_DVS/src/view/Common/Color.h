#pragma once
#include <iomanip>
#include <iostream>

struct Color
{
    u_int8_t r;
    u_int8_t g;
    u_int8_t b;
    u_int8_t a;

    Color(u_int8_t r, u_int8_t g, u_int8_t b, u_int8_t a)
        : r(r), g(g), b(b), a(a)
    {
    }

    Color(u_int32_t hexColor)
    {
        r = ((hexColor >> 16) & 0xFF);
        g = ((hexColor >> 8) & 0xFF);
        b = (hexColor & 0xFF);
        a = ((hexColor >> 24) & 0xFF);
    }
};