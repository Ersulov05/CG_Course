#pragma once
#include <cmath>

struct Point
{
    float x;
    float y;

    Point operator-(const Point &other) const
    {
        return Point(x - other.x, y - other.y);
    }

    Point operator-(float value) const
    {
        return Point(x - value, y - value);
    }

    Point operator+(const Point &other) const
    {
        return Point(x + other.x, y + other.y);
    }

    Point operator+(float value) const
    {
        return Point(x + value, y + value);
    }

    Point &operator+=(const Point &other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    // Оператор -=
    Point &operator-=(const Point &other)
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    bool operator==(const Point &other) const
    {
        // Используем небольшой эпсилон для сравнения float
        const float epsilon = 0.0001f;
        return (fabs(x - other.x) < epsilon) && (fabs(y - other.y) < epsilon);
    }

    // Оператор неравенства !=
    bool operator!=(const Point &other) const
    {
        return !(*this == other);
    }
};
