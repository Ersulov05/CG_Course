#pragma once
#include <cmath>

struct Vector3D
{
    float x, y, z;

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

    // Вектор - вектор = вектор
    Vector3D operator-(const Vector3D &other) const
    {
        return Vector3D(x - other.x, y - other.y, z - other.z);
    }

    // Вектор * скаляр = вектор
    Vector3D operator*(float scalar) const
    {
        return Vector3D(x * scalar, y * scalar, z * scalar);
    }

    // Вектор / скаляр = вектор
    Vector3D operator/(float scalar) const
    {
        return Vector3D(x / scalar, y / scalar, z / scalar);
    }
};