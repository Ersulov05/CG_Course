#pragma once
#include <cmath>
#include "./Vector3D.h"

struct Point3D
{
    float x, y, z;

    Point3D(float x = 0, float y = 0, float z = 0): x(x), y(y), z(z) {}

    Vector3D operator-(const Point3D &other) const
    {
        return Vector3D(x - other.x, y - other.y, z - other.z);
    }

    Point3D operator+(const Vector3D &other) const
    {
        return Point3D(x + other.x, y + other.y, z + other.z);
    }

    Point3D operator+(const Point3D &other) const
    {
        return Point3D(x + other.x, y + other.y, z + other.z);
    }

    Point3D operator*(float scalar) const
    {
        return Point3D(x * scalar, y * scalar, z * scalar);
    }

    Point3D operator/(float scalar) const
    {
        return Point3D(x / scalar, y / scalar, z / scalar);
    }
};
