#pragma once
#include <cmath>

struct Vector3D
{
    float x, y, z;

    Vector3D(float x = 0, float y = 0, float z = 0): x(x), y(y), z(z) {}

    float GetLength() const
    {
        return sqrt(x * x + y * y + z * z);
    }

    Vector3D Normalized() const
    {
        float len = GetLength();
        if (len > 0)
            return Vector3D(x / len, y / len, z / len);
        return *this;
    }

    Vector3D operator+(const Vector3D &other) const
    {
        return Vector3D(x + other.x, y + other.y, z + other.z);
    }

    Vector3D operator-(const Vector3D &other) const
    {
        return Vector3D(x - other.x, y - other.y, z - other.z);
    }

    Vector3D operator*(float scalar) const
    {
        return Vector3D(x * scalar, y * scalar, z * scalar);
    }

    Vector3D operator/(float scalar) const
    {
        return Vector3D(x / scalar, y / scalar, z / scalar);
    }

    Vector3D Cross(const Vector3D &other) const
    {
        return Vector3D(
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        );
    }
};