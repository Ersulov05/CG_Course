#pragma once
#include "./ICollisionObject.h"
#include "../../common/Point.h"
#include "../../common/TransformMatrix.h"

class CollisionSystem
{
public:
    static bool CheckCollision(const ICollisionObject &a, const ICollisionObject &b)
    {
        CollisionType typeA = a.GetType();
        CollisionType typeB = b.GetType();

        if (typeA == CollisionType::CIRCLE && typeB == CollisionType::CIRCLE)
        {
            return CircleCircleCollision(a, b);
        }

        if (typeA == CollisionType::RECT && typeB == CollisionType::RECT)
        {
            return OrientedRectCollision(a, b);
        }

        if (typeA == CollisionType::CIRCLE && typeB == CollisionType::RECT)
        {
            return CircleOrientedRectCollision(a, b);
        }
        if (typeA == CollisionType::RECT && typeB == CollisionType::CIRCLE)
        {
            return CircleOrientedRectCollision(b, a);
        }

        return false;
    }

private:
    static bool CircleCircleCollision(const ICollisionObject &a, const ICollisionObject &b)
    {
        Point diff = a.GetPosition() - b.GetPosition();
        float distSq = diff.x * diff.x + diff.y * diff.y;
        float radiusSum = a.GetRadius() + b.GetRadius();

        return distSq <= radiusSum * radiusSum;
    }

    static bool OrientedRectCollision(const ICollisionObject &a, const ICollisionObject &b)
    {
        // Получаем вершины прямоугольников
        std::vector<Point> vertsA = GetRectVertices(a);
        std::vector<Point> vertsB = GetRectVertices(b);

        // Проверяем 4 оси (2 от каждого прямоугольника)
        std::vector<Point> axes = {
            GetAxis(vertsA[0], vertsA[1]), // ось перпендикулярная первому ребру A
            GetAxis(vertsA[1], vertsA[2]), // ось перпендикулярная второму ребру A
            GetAxis(vertsB[0], vertsB[1]), // ось перпендикулярная первому ребру B
            GetAxis(vertsB[1], vertsB[2])  // ось перпендикулярная второму ребру B
        };

        // Проверяем каждую ось
        for (const auto &axis : axes)
        {
            float minA, maxA, minB, maxB;
            GetProjection(vertsA, axis, minA, maxA);
            GetProjection(vertsB, axis, minB, maxB);

            // Если проекции не пересекаются - столкновения нет
            if (maxA < minB || maxB < minA)
                return false;
        }

        // Если все оси показали пересечение - объекты столкнулись
        return true;
    }

    static bool CircleOrientedRectCollision(const ICollisionObject &circle, const ICollisionObject &rect)
    {
        std::vector<Point> vertices = GetRectVertices(rect);
        Point circlePos = circle.GetPosition();
        float radius = circle.GetRadius();

        for (const auto &vertex : vertices)
        {
            Point diff = circlePos - vertex;
            if (diff.x * diff.x + diff.y * diff.y <= radius * radius)
                return true;
        }

        for (size_t i = 0; i < vertices.size(); i++)
        {
            Point a = vertices[i];
            Point b = vertices[(i + 1) % vertices.size()];

            Point ab = b - a;
            Point ac = circlePos - a;

            float t = Dot(ac, ab) / Dot(ab, ab);
            t = std::max(0.0f, std::min(1.0f, t));

            Point closest = a + ab * t;
            Point diff = circlePos - closest;

            if (diff.x * diff.x + diff.y * diff.y <= radius * radius)
                return true;
        }

        return false;
    }

    static std::vector<Point> GetRectVertices(const ICollisionObject &rect)
    {
        float w = rect.GetWidth() / 2;
        float h = rect.GetHeight() / 2;

        std::vector<Point> vertices = {
            {-w, -h},
            {w, -h},
            {w, h},
            {-w, h}};

        TransformMatrix matrix;
        matrix.Translate(rect.GetPosition());
        matrix.Rotate(rect.GetRotation());

        for (int i = 0; i < 4; i++)
        {
            vertices[i] = matrix.Apply(vertices[i]);
        }

        return vertices;
    }

    static float Dot(const Point &a, const Point &b)
    {
        return a.x * b.x + a.y * b.y;
    }

    static Point Normalize(const Point &v)
    {
        float len = sqrt(v.x * v.x + v.y * v.y);
        if (len > 0)
            return {v.x / len, v.y / len};
        return v;
    }

    static Point GetAxis(const Point &p1, const Point &p2)
    {
        Point edge = p2 - p1;
        Point axis = {-edge.y, edge.x};

        return Normalize(axis);
    }

    static void GetProjection(const std::vector<Point> &vertices, const Point &axis,
                              float &min, float &max)
    {
        min = max = Dot(vertices[0], axis);

        for (size_t i = 1; i < vertices.size(); i++)
        {
            float proj = Dot(vertices[i], axis);
            min = std::min(min, proj);
            max = std::max(max, proj);
        }
    }
};