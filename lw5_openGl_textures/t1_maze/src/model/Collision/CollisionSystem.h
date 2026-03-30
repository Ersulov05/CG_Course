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

        Vector3D axisAX = (vertsA[0] - vertsA[1]).Normalized();
        Vector3D axisAY = (vertsA[0] - vertsA[2]).Normalized();
        Vector3D axisAZ = (vertsA[0] - vertsA[3]).Normalized();
        
        Vector3D axisBX = (vertsB[0] - vertsB[1]).Normalized();
        Vector3D axisBY = (vertsB[0] - vertsB[2]).Normalized();
        Vector3D axisBZ = (vertsB[0] - vertsB[3]).Normalized();

        std::vector<Vector3D> axes = {
            axisAX, axisAY, axisAZ,
            axisBX, axisBY, axisBZ,
            
            axisAX.Cross(axisBX).Normalized(),
            axisAX.Cross(axisBY).Normalized(),
            axisAX.Cross(axisBZ).Normalized(),
            axisAY.Cross(axisBX).Normalized(),
            axisAY.Cross(axisBY).Normalized(),
            axisAY.Cross(axisBZ).Normalized(),
            axisAZ.Cross(axisBX).Normalized(),
            axisAZ.Cross(axisBY).Normalized(),
            axisAZ.Cross(axisBZ).Normalized()
        };

        axes.erase(std::remove_if(axes.begin(), axes.end(),
            [](const Vector3D& axis) {
                return axis.GetLength() < 1e-6;
            }), axes.end());

        for (const auto &axis : axes)
        {
            float minA, maxA, minB, maxB;
            GetProjection(vertsA, axis, minA, maxA);
            GetProjection(vertsB, axis, minB, maxB);

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

    static void GetProjection(
        const std::vector<Point3D> &vertices, 
        const Vector3D &axis,
        float &min, 
        float &max)
    {
        min = max = ToVector(vertices[0]).Dot(axis);

        for (size_t i = 1; i < vertices.size(); i++)
        {
            float proj = ToVector(vertices[i]).Dot(axis);
            min = std::min(min, proj);
            max = std::max(max, proj);
        }
    }
};