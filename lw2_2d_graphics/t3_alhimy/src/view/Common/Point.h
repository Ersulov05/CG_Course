#pragma once

struct Point
{
    float x;
    float y;

    Point operator-(const Point &other) const
    {
        return Point(x - other.x, y - other.y);
    }

    // Оператор сложения
    Point operator+(const Point &other) const
    {
        return Point(x + other.x, y + other.y);
    }

    // Оператор +=
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
};
