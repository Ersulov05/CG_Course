#pragma once
#include "./Point3D.h"
#include "./Vector3D.h"

inline Vector3D ToVector(const Point3D &p)
{
    return Vector3D(p.x, p.y, p.z);
}

inline Point3D ToPoint(const Vector3D &v)
{
    return Point3D(v.x, v.y, v.z);
}