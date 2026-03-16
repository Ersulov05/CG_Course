#pragma once

struct Size3D {
    float width;
    float height;
    float depth;

    Size3D(float width = 0, float height = 0, float depth = 0): width(width), height(height), depth(depth) {}

    Size3D operator*(float scalar) const
    {
        return Size3D(width * scalar, height * scalar, depth * scalar);
    }

    Size3D operator/(float scalar) const
    {
        return Size3D(width / scalar, height / scalar, depth / scalar);
    }
};