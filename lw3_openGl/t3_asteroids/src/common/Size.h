#pragma once

struct Size
{
    float width;
    float height;

    Size() : width(0), height(0) {}
    Size(float w, float h) : width(w), height(h) {}

    Size operator+(const Size &other) const
    {
        return Size(width + other.width, height + other.height);
    }

    Size &operator+=(const Size &other)
    {
        width += other.width;
        height += other.height;
        return *this;
    }

    Size operator-(const Size &other) const
    {
        return Size(width - other.width, height - other.height);
    }

    Size &operator-=(const Size &other)
    {
        width -= other.width;
        height -= other.height;
        return *this;
    }
};