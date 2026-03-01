#pragma once
#include "./ICollisionObject.h"
#include "../../common/Point.h"

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
        std::vector<Point> verticesA = GetRectVertices(a);
        std::vector<Point> verticesB = GetRectVertices(b);

        std::vector<Point> axes;

        for (size_t i = 0; i < verticesA.size(); i++)
        {
            Point edge = verticesA[(i + 1) % verticesA.size()] - verticesA[i];
            Point axis = {-edge.y, edge.x};
            axes.push_back(Normalize(axis));
        }

        for (size_t i = 0; i < verticesB.size(); i++)
        {
            Point edge = verticesB[(i + 1) % verticesB.size()] - verticesB[i];
            Point axis = {-edge.y, edge.x};
            axes.push_back(Normalize(axis));
        }

        for (const auto &axis : axes)
        {
            float minA, maxA, minB, maxB;
            ProjectPolygon(verticesA, axis, minA, maxA);
            ProjectPolygon(verticesB, axis, minB, maxB);

            if (maxA < minB || maxB < minA)
                return false;
        }

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
        std::vector<Point> vertices(4);
        float w = rect.GetWidth() / 2;
        float h = rect.GetHeight() / 2;
        float angle = rect.GetRotation() * M_PI / 180.0f;
        Point center = rect.GetPosition();

        Point corners[4] = {
            {-w, -h},
            {w, -h},
            {w, h},
            {-w, h}};

        for (int i = 0; i < 4; i++)
        {
            float x = corners[i].x * cos(angle) - corners[i].y * sin(angle);
            float y = corners[i].x * sin(angle) + corners[i].y * cos(angle);
            vertices[i] = {center.x + x, center.y + y};
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

    static void ProjectPolygon(const std::vector<Point> &vertices, const Point &axis,
                               float &min, float &max)
    {
        min = Dot(vertices[0], axis);
        max = min;

        for (size_t i = 1; i < vertices.size(); i++)
        {
            float proj = Dot(vertices[i], axis);
            min = std::min(min, proj);
            max = std::max(max, proj);
        }
    }
};