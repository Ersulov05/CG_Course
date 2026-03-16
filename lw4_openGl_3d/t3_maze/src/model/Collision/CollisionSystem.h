#pragma once
#include "./BoxCollision.h"
#include "../../common/Geometry.h"
#include "../../common/TransformMatrix.h"

class CollisionSystem
{
public:
    static bool CheckCollision(const BoxCollision &a, const BoxCollision &b)
    {
        return OrientedBoxCollision(a, b);
    }

private:
    static bool OrientedBoxCollision(const BoxCollision &a, const BoxCollision &b)
    {
        std::vector<Point3D> vertsA = GetBoxVertices(a);
        std::vector<Point3D> vertsB = GetBoxVertices(b);

        Vector3D axisAX = (vertsA[0] - vertsA[1]).Normalized();  // Ось X бокса A
    Vector3D axisAY = (vertsA[0] - vertsA[2]).Normalized();  // Ось Y бокса A
    Vector3D axisAZ = (vertsA[0] - vertsA[3]).Normalized();  // Ось Z бокса A
    
    Vector3D axisBX = (vertsB[0] - vertsB[1]).Normalized();  // Ось X бокса B
    Vector3D axisBY = (vertsB[0] - vertsB[2]).Normalized();  // Ось Y бокса B
    Vector3D axisBZ = (vertsB[0] - vertsB[3]).Normalized();  // Ось Z бокса B

    // Все 15 осей
    std::vector<Vector3D> axes = {
        // 3 оси бокса A
        axisAX, axisAY, axisAZ,
        
        // 3 оси бокса B
        axisBX, axisBY, axisBZ,
        
        // 9 cross-произведений (каждая ось A × каждая ось B)
        Cross(axisAX, axisBX).Normalized(),
        Cross(axisAX, axisBY).Normalized(),
        Cross(axisAX, axisBZ).Normalized(),
        Cross(axisAY, axisBX).Normalized(),
        Cross(axisAY, axisBY).Normalized(),
        Cross(axisAY, axisBZ).Normalized(),
        Cross(axisAZ, axisBX).Normalized(),
        Cross(axisAZ, axisBY).Normalized(),
        Cross(axisAZ, axisBZ).Normalized()
    };

    // Убираем нулевые оси (параллельные вектора)
    axes.erase(std::remove_if(axes.begin(), axes.end(),
        [](const Vector3D& axis) {
            return axis.GetLength() < 1e-6;
        }), axes.end());


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

        return true;
    }

    static std::vector<Point3D> GetBoxVertices(const BoxCollision &box)
    {
        auto halfSize = box.GetSize() / 2;
        auto halfWidth = halfSize.width;
        auto halfHeight = halfSize.height;
        auto halfDepth = halfSize.depth;

        std::vector<Point3D> vertices = {
            {-halfWidth, -halfHeight, -halfDepth},
            {halfWidth, -halfHeight, -halfDepth},
            {-halfWidth, halfHeight, -halfDepth},
            {-halfWidth, -halfHeight, halfDepth},
            
            {-halfWidth, halfHeight, halfDepth},
            {halfWidth, halfHeight, -halfDepth},
            {halfWidth, halfHeight, halfDepth},
            {halfWidth, -halfHeight, halfDepth}
            };

        auto boxRotation = box.GetRotation();
        TransformMatrix matrix;
        matrix.Translate(ToVector(box.GetPosition()));
        matrix.RotateX(boxRotation.x);
        matrix.RotateY(boxRotation.y);
        matrix.RotateZ(boxRotation.z);

        for (int i = 0; i < 8; i++)
        {
            vertices[i] = matrix.Apply(vertices[i]);
        }

        return vertices;
    }

    static float Dot(const Vector3D &a, const Vector3D &b)
    {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    static Vector3D Cross(const Vector3D &a, const Vector3D &b)
    {
        return {
            a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x
        };
    }


    static void GetProjection(const std::vector<Point3D> &vertices, const Vector3D &axis,
                              float &min, float &max)
    {
        min = max = Dot(ToVector(vertices[0]), axis);

        for (size_t i = 1; i < vertices.size(); i++)
        {
            float proj = Dot(ToVector(vertices[i]), axis);
            min = std::min(min, proj);
            max = std::max(max, proj);
        }
    }
};