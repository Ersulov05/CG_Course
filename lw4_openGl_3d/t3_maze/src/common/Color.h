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

    Color(float r, float g, float b, float a = 1.0f)
        : r(r), g(g), b(b), a(a)
    {
    }


    Color(u_int32_t hexColor)
    {
        r = ((hexColor >> 24) & 0xFF) / 255.0f;
        g = ((hexColor >> 16) & 0xFF) / 255.0f;
        b = ((hexColor >> 8) & 0xFF) / 255.0f;
        a = (hexColor & 0xFF) / 255.0f;
    }

    static Color FromHSV(float hue, float saturation, float value)
    {
        // Нормализуем hue в диапазон 0-360
        hue = fmod(hue, 360.0f);
        if (hue < 0) hue += 360.0f;
        
        float r, g, b;
        
        if (saturation <= 0.0f) {
            r = g = b = value;
        } else {
            float h = hue / 60.0f;
            int i = (int)h;
            float f = h - i;
            float p = value * (1.0f - saturation);
            float q = value * (1.0f - saturation * f);
            float t = value * (1.0f - saturation * (1.0f - f));
            
            switch (i) {
                case 0: r = value; g = t; b = p; break;
                case 1: r = q; g = value; b = p; break;
                case 2: r = p; g = value; b = t; break;
                case 3: r = p; g = q; b = value; break;
                case 4: r = t; g = p; b = value; break;
                default: // case 5 и все остальные (включая 6)
                    r = value; g = p; b = q; break;
            }
        }
        
        return Color(r, g, b, 1.0f);
    }
};