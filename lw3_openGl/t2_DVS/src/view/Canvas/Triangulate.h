#pragma once
#include "../Common/Point.h"
#include "../Common/utils.h"
#include <vector>
#include <cmath>
#include <algorithm>
#include <stack>
#include <map>

struct Triangle
{
    Point a;
    Point b;
    Point c;

    Triangle(const Point &a, const Point &b, const Point &c)
        : a(a), b(b), c(c) {}
};

class Triangulate
{
public:
    static std::vector<unsigned int> Process(const std::vector<Point> &polygon)
    {
        if (polygon.size() < 3)
            return {};

        if (IsConvex(polygon))
        {
            return TriangulateConvex(polygon);
        }

        return TriangulateUnconvex(polygon);
    }

private:
    static float CrossProduct(const Point &p1, const Point &p2, const Point &p3)
    {
        return (p2.x - p1.x) * (p3.y - p2.y) - (p2.y - p1.y) * (p3.x - p2.x);
    }

    static float DotProduct(const Point &p1, const Point &p2, const Point &p3)
    {
        return (p2.x - p1.x) * (p3.x - p2.x) + (p2.y - p1.y) * (p3.y - p2.y);
    }

    static float CalculateArea(const std::vector<Point> &polygon)
    {
        float area = 0;
        for (size_t i = 0; i < polygon.size(); i++)
        {
            size_t j = (i + 1) % polygon.size();
            area += polygon[i].x * polygon[j].y - polygon[j].x * polygon[i].y;
        }
        return area;
    }

    static bool IsConvex(const std::vector<Point> &polygon)
    {
        int n = polygon.size();
        if (n < 3)
            return false;

        bool isPositive = false;

        for (int i = 0; i < n; i++)
        {
            int prev = (i - 1 + n) % n;
            int next = (i + 1) % n;

            float cross = CrossProduct(polygon[prev], polygon[i], polygon[next]);

            if (i == 0)
                isPositive = cross > 0;
            else if ((cross > 0) != isPositive)
                return false;
        }

        return true;
    }

    static std::vector<unsigned int> TriangulateConvex(const std::vector<Point> &polygon)
    {
        std::vector<unsigned int> indices;
        int n = polygon.size();

        for (int i = 1; i < n - 1; i++)
        {
            indices.push_back(0);
            indices.push_back(i);
            indices.push_back(i + 1);
        }

        return indices;
    }

    static bool IsConvexVertex(const Point &prev, const Point &curr, const Point &next)
    {
        return CrossProduct(prev, curr, next) > 0;
    }

    static bool IsClockwise(const std::vector<Point> &polygon)
    {
        return CalculateArea(polygon) < 0;
    }

    static std::vector<unsigned int> TriangulateUnconvex(std::vector<Point> polygon)
    {
        std::vector<unsigned int> indices;
        if (polygon.size() < 3)
            return indices;

        if (IsClockwise(polygon))
        {
            std::reverse(polygon.begin(), polygon.end());
        }

        std::vector<int> vertices(polygon.size());
        for (int i = 0; i < polygon.size(); i++)
            vertices[i] = i;

        while (vertices.size() > 3)
        {
            if (!CutEar(polygon, vertices, indices))
                break;
        }

        if (vertices.size() == 3)
        {
            indices.push_back(vertices[0]);
            indices.push_back(vertices[1]);
            indices.push_back(vertices[2]);
        }

        return indices;
    }

    static bool IsEarVertex(
        const std::vector<Point> &polygon,
        const std::vector<int> &vertices,
        int iPrev, int i, int iNext)
    {
        const Point &pPrev = polygon[vertices[iPrev]];
        const Point &pCurr = polygon[vertices[i]];
        const Point &pNext = polygon[vertices[iNext]];
        bool isEar = true;

        if (!IsConvexVertex(pPrev, pCurr, pNext))
        {
            return false;
        }
        else
        {
            for (int j = 0; j < vertices.size(); j++)
            {
                if (j == iPrev || j == i || j == iNext)
                    continue;

                if (IsPointInTriangle(polygon[vertices[j]], pPrev, pCurr, pNext))
                {
                    return false;
                }
            }
        }

        return true;
    }

    static bool CutEar(
        const std::vector<Point> &polygon,
        std::vector<int> &vertices,
        std::vector<unsigned int> &indices)
    {
        auto size = vertices.size();
        for (int i = 0; i < size; i++)
        {
            int iPrev = (i - 1 + size) % size;
            int iNext = (i + 1) % size;

            if (IsEarVertex(polygon, vertices, iPrev, i, iNext))
            {
                indices.push_back(vertices[iPrev]);
                indices.push_back(vertices[i]);
                indices.push_back(vertices[iNext]);

                vertices.erase(vertices.begin() + i);
                return true;
            }
        }
        return false;
    }
};