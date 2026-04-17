#pragma once
#include "../../common/Geometry.h"
#include "../../common/TransformMatrix.h"
#include "../Constants.h"
#include <optional>

#include "../Tank/Tank.h"
#include "../Map/Wall/Wall.h"

class CollisionSystem
{
public:
    static bool CheckAndHandleCollision(Tank &tank, const Wall &wall)
    {
        Size3D rotatedTankSize = GetRotatedSize(tank.GetSize(), tank.GetRotation());
        auto collisionData = CheckCollision(wall.GetPosition(), wall.GetSize(), tank.GetPosition(), rotatedTankSize);

        if (!collisionData.has_value()) {
            return false;
        }

        CorrectTankBySpeedAndNormal(tank, *collisionData);

        collisionData = CheckCollision(wall.GetPosition(), wall.GetSize(), tank.GetPosition(), rotatedTankSize);
        if (!collisionData.has_value()) {
            return true;
        }

        CorrectTankByNormal(tank, *collisionData);
        
        return true;
    }

    static bool CheckAndHandleCollision(Shell& shell, Wall& wall)
    {
        Size3D rotatedShellSize = GetRotatedSize(shell.GetSize(), shell.GetRotation());
        auto collisionData = CheckCollision(wall.GetPosition(), wall.GetSize(), shell.GetPosition(), rotatedShellSize);

        if (!collisionData.has_value()) {
            return false;
        }
        return true;
    }

private:
    struct CollisionData {
        Vector3D normal;
        float overlap;
    };

    struct Bound {
        float left;
        float right;
        float front;
        float back;
    };

    static void CorrectTankBySpeedAndNormal(Tank& tank, const CollisionData& collisionData)
    {
        Vector3D speed = tank.GetSpeed();
        Vector3D normal = collisionData.normal;
        float overlap = collisionData.overlap + 0.01;
        
        Vector3D correction(0, 0, 0);
        
        if (normal.x != 0) {
            if (speed.x * normal.x > 0) 
            {
                correction.x = normal.x * overlap;
            }
        }
        
        if (normal.z != 0) {
            if (speed.z * normal.z > 0) {
                correction.z = normal.z * overlap;
            }
        }
        
        Point3D newPosition = tank.GetPosition();
        newPosition.x += correction.x;
        newPosition.z += correction.z;
        tank.SetPosition(newPosition);
        
        Vector3D newSpeed = speed;
        if (correction.x != 0) {
            newSpeed.x = 0;
        }
        if (correction.z != 0) {
            newSpeed.z = 0;
        }
        tank.SetSpeed(newSpeed);
    }

    static void CorrectTankByNormal(Tank& tank, const CollisionData& collisionData)
    {
        Vector3D speed = tank.GetSpeed();
        Vector3D normal = collisionData.normal;
        float overlap = collisionData.overlap + 0.01;
        
        Vector3D correction = normal * overlap;
        
        Point3D newPosition = tank.GetPosition();
        newPosition.x += correction.x;
        newPosition.z += correction.z;
        tank.SetPosition(newPosition);
        
        // Vector3D newSpeed = speed;
        // if (normal.x != 0 && speed.x * normal.x > 0) {
        //     newSpeed.x = 0;
        // }
        // if (normal.z != 0 && speed.z * normal.z > 0) {
        //     newSpeed.z = 0;
        // }
        // tank.SetSpeed(newSpeed);
    }

    static Size3D GetRotatedSize(const Size3D& originalSize, const Quaternion3D& rotation)
    {
        float yaw = rotation.GetYaw();
        
        float absCos = std::abs(std::cos(yaw));
        float absSin = std::abs(std::sin(yaw));
        
        float worldWidth = originalSize.width * absCos + originalSize.depth * absSin;
        float worldDepth = originalSize.width * absSin + originalSize.depth * absCos;
        
        return Size3D(worldWidth, originalSize.height, worldDepth);
    }

    static std::optional<CollisionData> CheckCollision(
        const Point3D& firstBlockPos, const Size3D& firstBlockSize,
        const Point3D& secondBlockPos, const Size3D& secondBlockSize
    )
    {
        Bound firstBound = GetBound(firstBlockPos, firstBlockSize);
        Bound secondBound = GetBound(secondBlockPos, secondBlockSize);
        
        bool overlapX = (firstBound.left <= secondBound.right) && (firstBound.right >= secondBound.left);
        bool overlapZ = (firstBound.front <= secondBound.back) && (firstBound.back >= secondBound.front);
        
        if (!overlapX || !overlapZ) {
            return std::nullopt;
        }
        
        float overlapXDepth = std::min(firstBound.right - secondBound.left, secondBound.right - firstBound.left);
        float overlapZDepth = std::min(firstBound.back - secondBound.front, secondBound.back - firstBound.front);
        
        bool crossedLeft = (secondBound.right > firstBound.left && secondBound.left < firstBound.left);
        bool crossedRight = (secondBound.left < firstBound.right && secondBound.right > firstBound.right);
        bool crossedFront = (secondBound.back > firstBound.front && secondBound.front < firstBound.front);
        bool crossedBack = (secondBound.front < firstBound.back && secondBound.back > firstBound.back);
        
        Vector3D normal(0, 0, 0);
        
        if (crossedLeft && !crossedRight) {
            normal.x = -1;
        } else if (crossedRight && !crossedLeft) {
            normal.x = 1;
        } else if (crossedLeft && crossedRight) {
            normal.x = (secondBlockPos.x < firstBlockPos.x) ? -1 : 1;
        }
        
        if (crossedFront && !crossedBack) {
            normal.z = -1;
        } else if (crossedBack && !crossedFront) {
            normal.z = 1;
        } else if (crossedFront && crossedBack) {
            normal.z = (secondBlockPos.z < firstBlockPos.z) ? -1 : 1;
        }
        
        if (normal.x != 0 && normal.z != 0) {
            normal = normal.Normalized();
        }
        
        float overlap = (overlapXDepth < overlapZDepth) ? overlapXDepth : overlapZDepth;
        return CollisionData{normal, overlap};
    }

    static Bound GetBound(const Point3D& Pos, const Size3D& Size)
    {
        return Bound{
            Pos.x - Size.width / 2,
            Pos.x + Size.width / 2,
            Pos.z - Size.depth / 2,
            Pos.z + Size.depth / 2
        };
    }
};