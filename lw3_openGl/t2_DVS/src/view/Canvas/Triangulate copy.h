// #pragma once
// #include "../Common/Point.h"
// #include <vector>
// #include <cmath>
// #include <algorithm>
// #include <stack>

// class Triangulate
// {
// public:
//     static std::vector<unsigned int> Process(const std::vector<Point> &polygon)
//     {
//         if (polygon.size() < 3)
//             return {};

//         // Используем алгоритм "отрезания ушей" (ear clipping)
//         return TriangulateEarClipping(polygon);
//     }

// private:
//     static float CrossProduct(const Point &p1, const Point &p2, const Point &p3)
//     {
//         return (p2.x - p1.x) * (p3.y - p2.y) - (p2.y - p1.y) * (p3.x - p2.x);
//     }

//     static float CalculateArea(const std::vector<Point> &polygon)
//     {
//         float area = 0;
//         for (size_t i = 0; i < polygon.size(); i++)
//         {
//             size_t j = (i + 1) % polygon.size();
//             area += polygon[i].x * polygon[j].y - polygon[j].x * polygon[i].y;
//         }
//         return area;
//     }

//     // Проверка, является ли вершина выпуклой (внутренний угол < 180°)
//     static bool IsConvexVertex(const Point &prev, const Point &curr, const Point &next, float polygonArea)
//     {
//         float cross = CrossProduct(prev, curr, next);
//         // Для полигона с положительной площадью (против часовой стрелки)
//         // выпуклая вершина имеет cross > 0
//         return (polygonArea > 0) ? (cross > 0) : (cross < 0);
//     }

//     // Проверка, находится ли точка внутри треугольника
//     static bool IsPointInTriangle(const Point &p, const Point &a, const Point &b, const Point &c)
//     {
//         float areaABC = std::abs(CrossProduct(a, b, c));
//         float areaPBC = std::abs(CrossProduct(p, b, c));
//         float areaAPC = std::abs(CrossProduct(a, p, c));
//         float areaABP = std::abs(CrossProduct(a, b, p));

//         const float epsilon = 0.0001f;
//         return std::abs(areaABC - (areaPBC + areaAPC + areaABP)) < epsilon;
//     }

//     static std::vector<unsigned int> TriangulateEarClipping(const std::vector<Point> &polygon)
//     {
//         std::vector<unsigned int> indices;
//         int n = polygon.size();

//         if (n < 3)
//             return indices;

//         // Создаем список вершин
//         std::vector<int> vertices(n);
//         for (int i = 0; i < n; i++)
//             vertices[i] = i;

//         float polygonArea = CalculateArea(polygon);

//         int currentVertices = n;
//         int i = 0;

//         while (currentVertices > 3)
//         {
//             // Получаем три последовательные вершины
//             int i0 = vertices[i];
//             int i1 = vertices[(i + 1) % currentVertices];
//             int i2 = vertices[(i + 2) % currentVertices];

//             const Point &p0 = polygon[i0];
//             const Point &p1 = polygon[i1];
//             const Point &p2 = polygon[i2];

//             // Проверяем, является ли средняя вершина "ухом"
//             bool isEar = true;

//             // 1. Проверяем, что вершина выпуклая
//             if (!IsConvexVertex(p0, p1, p2, polygonArea))
//             {
//                 isEar = false;
//             }
//             else
//             {
//                 // 2. Проверяем, нет ли других вершин внутри треугольника
//                 for (int j = 0; j < currentVertices; j++)
//                 {
//                     int vj = vertices[j];
//                     if (vj == i0 || vj == i1 || vj == i2)
//                         continue;

//                     if (IsPointInTriangle(polygon[vj], p0, p1, p2))
//                     {
//                         isEar = false;
//                         break;
//                     }
//                 }
//             }

//             if (isEar)
//             {
//                 // Добавляем треугольник
//                 indices.push_back(i0);
//                 indices.push_back(i1);
//                 indices.push_back(i2);

//                 // Удаляем среднюю вершину
//                 vertices.erase(vertices.begin() + ((i + 1) % currentVertices));
//                 currentVertices--;

//                 // Возвращаемся на шаг назад
//                 i = (i == 0) ? currentVertices - 1 : i - 1;
//             }
//             else
//             {
//                 i = (i + 1) % currentVertices;
//             }
//         }

//         // Добавляем последний треугольник
//         indices.push_back(vertices[0]);
//         indices.push_back(vertices[1]);
//         indices.push_back(vertices[2]);

//         return indices;
//     }
// };

#pragma once
#include "../Common/Point.h"
#include <vector>
#include <cmath>
#include <algorithm>
#include <stack>
#include <map>

class Triangulate
{
public:
    static std::vector<unsigned int> Process(const std::vector<Point> &polygon)
    {
        if (polygon.size() < 3)
            return {};

        // Проверяем, является ли полигон выпуклым
        if (IsConvex(polygon))
        {
            return TriangulateConvex(polygon);
        }

        // Для невыпуклых используем улучшенный алгоритм
        return TriangulateAdvanced(polygon);
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

    // Проверка, является ли вершина выпуклой
    static bool IsConvexVertex(const Point &prev, const Point &curr, const Point &next)
    {
        return CrossProduct(prev, curr, next) > 0;
    }

    // Проверка, находится ли точка внутри треугольника
    static bool IsPointInTriangle(const Point &p, const Point &a, const Point &b, const Point &c)
    {
        // Используем метод барицентрических координат
        float v0x = c.x - a.x;
        float v0y = c.y - a.y;
        float v1x = b.x - a.x;
        float v1y = b.y - a.y;
        float v2x = p.x - a.x;
        float v2y = p.y - a.y;

        float dot00 = v0x * v0x + v0y * v0y;
        float dot01 = v0x * v1x + v0y * v1y;
        float dot02 = v0x * v2x + v0y * v2y;
        float dot11 = v1x * v1x + v1y * v1y;
        float dot12 = v1x * v2x + v1y * v2y;

        float invDenom = 1.0f / (dot00 * dot11 - dot01 * dot01);
        float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
        float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

        return (u >= 0) && (v >= 0) && (u + v <= 1);
    }

    // Проверка, является ли полигон ориентированным по часовой стрелке
    static bool IsClockwise(const std::vector<Point> &polygon)
    {
        return CalculateArea(polygon) < 0;
    }

    // Улучшенный алгоритм триангуляции
    static std::vector<unsigned int> TriangulateAdvanced(const std::vector<Point> &polygon)
    {
        std::vector<unsigned int> indices;
        int n = polygon.size();

        if (n < 3)
            return indices;

        // Копируем полигон и при необходимости разворачиваем
        std::vector<Point> points = polygon;
        if (IsClockwise(polygon))
        {
            std::reverse(points.begin(), points.end());
        }

        // Создаем список активных вершин
        std::vector<bool> active(n, true);
        std::vector<int> prev(n), next(n);

        for (int i = 0; i < n; i++)
        {
            prev[i] = (i - 1 + n) % n;
            next[i] = (i + 1) % n;
        }

        int remaining = n;
        int i = 0;

        while (remaining > 3)
        {
            // Пропускаем неактивные вершины
            while (!active[i])
                i = (i + 1) % n;

            int iPrev = prev[i];
            int iNext = next[i];

            // Пропускаем неактивные соседей
            while (!active[iPrev])
                iPrev = prev[iPrev];
            while (!active[iNext])
                iNext = next[iNext];

            const Point &pPrev = points[iPrev];
            const Point &pCurr = points[i];
            const Point &pNext = points[iNext];

            // Проверяем, является ли вершина "ухом"
            bool isEar = true;

            // 1. Проверяем, что вершина выпуклая
            if (!IsConvexVertex(pPrev, pCurr, pNext))
            {
                isEar = false;
            }
            else
            {
                // 2. Проверяем, нет ли других вершин внутри треугольника
                for (int j = 0; j < n; j++)
                {
                    if (!active[j] || j == iPrev || j == i || j == iNext)
                        continue;

                    if (IsPointInTriangle(points[j], pPrev, pCurr, pNext))
                    {
                        isEar = false;
                        break;
                    }
                }
            }

            if (isEar)
            {
                // Добавляем треугольник
                indices.push_back(iPrev);
                indices.push_back(i);
                indices.push_back(iNext);

                // Удаляем текущую вершину
                active[i] = false;
                remaining--;

                // Обновляем связи
                next[iPrev] = iNext;
                prev[iNext] = iPrev;
            }

            i = (i + 1) % n;
        }

        // Добавляем последний треугольник
        std::vector<int> lastVertices;
        for (int j = 0; j < n; j++)
        {
            if (active[j])
                lastVertices.push_back(j);
        }

        if (lastVertices.size() == 3)
        {
            indices.push_back(lastVertices[0]);
            indices.push_back(lastVertices[1]);
            indices.push_back(lastVertices[2]);
        }

        return indices;
    }
};