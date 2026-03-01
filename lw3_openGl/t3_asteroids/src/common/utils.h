#pragma once
#include "./Point.h"
#include <cmath>

inline static bool IsPointInTriangle(const Point &p, const Point &a, const Point &b, const Point &c)
{
    float v0x = c.x - a.x;
    float v0y = c.y - a.y;
    float v1x = b.x - a.x;
    float v1y = b.y - a.y;
    float v2x = p.x - a.x;
    float v2y = p.y - a.y;

    float dot00 = v0x * v0x + v0y * v0y;
    float dot01 = v0x * v1x + v0y * v1y;
    float dot02 = v0x * v2x + v0y * v2y;
    float dot11 = v1x * v1x + v1y * v1y;
    float dot12 = v1x * v2x + v1y * v2y;

    float invDenom = 1.0f / (dot00 * dot11 - dot01 * dot01);
    float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
    float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

    return (u >= 0) && (v >= 0) && (u + v <= 1);
}

constexpr float DEG_TO_RAD = M_PI / 180.0f;
constexpr float RAD_TO_DEG = 180.0f / M_PI;

inline float ToRadians(float degrees)
{
    return degrees * DEG_TO_RAD;
}

inline float ToDegrees(float radians)
{
    return radians * RAD_TO_DEG;
}