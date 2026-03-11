#pragma once
#include <iomanip>
#include <iostream>

struct Color
{
    float r;
    float g;
    float b;
    float a;
    Color(){};

    Color(u_int8_t r, u_int8_t g, u_int8_t b, u_int8_t a)
        : r(r / 255.0f), g(g / 255.0f), b(b / 255.0f), a(a / 255.0f)
    {
    }

    Color(u_int32_t hexColor)
    {
        r = ((hexColor >> 24) & 0xFF) / 255.0f;
        g = ((hexColor >> 16) & 0xFF) / 255.0f;
        b = ((hexColor >> 8) & 0xFF) / 255.0f;
        a = (hexColor & 0xFF) / 255.0f;
    }
};