#pragma once
#include <cmath>
#include <vector>

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

Point RotatePoint(const Point &p, float angle, const Point &center)
{
    float s = sin(angle / 180 * 3.14);
    float c = cos(angle / 180 * 3.14);

    float dx = p.x - center.x;
    float dy = p.y - center.y;

    float xnew = dx * c - dy * s;
    float ynew = dx * s + dy * c;

    return Point(center.x + xnew, center.y + ynew);
}

Point TransformPoint(const Point &p, const Point &center, float angleDegrees, float scale = 1.0f)
{
    // Конвертация градусов в радианы
    const float pi = 3.14159265359f;
    float radians = angleDegrees * pi / 180.0f;

    float s = sin(radians);
    float c = cos(radians);

    // Смещение относительно центра
    float dx = (p.x - center.x) * scale;
    float dy = (p.y - center.y) * scale;

    // Поворот
    float xnew = dx * c - dy * s;
    float ynew = dx * s + dy * c;

    return Point(center.x + xnew, center.y + ynew);
}

std::vector<Point> TransformPoints(const std::vector<Point> &points, const Point &center, float rotation, float scale)
{
    std::vector<Point> transformedPoints;

    for (const auto &p : points)
    {
        transformedPoints.push_back(TransformPoint(p, center, rotation, scale));
    }

    return transformedPoints;
}
