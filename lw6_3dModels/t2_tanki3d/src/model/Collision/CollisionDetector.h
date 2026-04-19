#pragma once
#include "../../common/Geometry.h"
#include "../../common/TransformMatrix.h"
#include "../Constants.h"
#include <optional>

#include "../Tank/Tank.h"
#include "../Map/Wall/Wall.h"
#include "../Bonus/Bonus.h"

struct CollisionData {
    Vector3D normal;
    float overlap;
};

class CollisionDetector
{
public:
    static bool Detect(const Shell& shell, const Wall& wall)
    {
        if (!shell.IsAlive() || wall.GetType() == WallType::Empty) {
            return false;
        }
        Size3D rotatedShellSize = GetRotatedSize(shell.GetSize(), shell.GetRotation());
        auto collisionData = CheckCollision(wall.GetPosition(), wall.GetSize(), shell.GetPosition(), rotatedShellSize);

        return collisionData.has_value();
    }

    static bool Detect(Shell& shell, Headquarters& headquarters)
    {
        if (!shell.IsAlive()) {
            return false;
        }

        Size3D rotatedShellSize = GetRotatedSize(shell.GetSize(), shell.GetRotation());
        Size3D rotatedHeadquartersSize = GetRotatedSize(headquarters.GetSize(), headquarters.GetRotation());

        auto collisionData = CheckCollision(headquarters.GetPosition(), rotatedHeadquartersSize, shell.GetPosition(), rotatedShellSize);
        return collisionData.has_value();
    }

    static bool Detect(const Shell& shell, const std::shared_ptr<Tank> tank)
    {
        if (!shell.IsAlive() || tank->GetHealth() == 0) {
            return false;
        }

        auto owner = shell.GetOwner().lock();
        if (owner && owner.get() == tank.get()) {
            return false;
        }

        Size3D rotatedShellSize = GetRotatedSize(shell.GetSize(), shell.GetRotation());
        Size3D rotatedTankSize = GetRotatedSize(tank->GetSize(), tank->GetRotation());

        auto collisionData = CheckCollision(tank->GetPosition(), rotatedTankSize, shell.GetPosition(), rotatedShellSize);

        if (!collisionData.has_value()) {
            return false;
        }
        return true;
    }

    static std::optional<CollisionData> Detect(const std::shared_ptr<Tank> firstTank, const std::shared_ptr<Tank> secondTank)
    {
        Size3D rotatedFirstTankSize = GetRotatedSize(firstTank->GetSize(), firstTank->GetRotation());
        Size3D rotatedSecondTankSize = GetRotatedSize(secondTank->GetSize(), secondTank->GetRotation());

        return CheckCollision(secondTank->GetPosition(), rotatedSecondTankSize, firstTank->GetPosition(), rotatedFirstTankSize);
    }

    static std::optional<CollisionData> Detect(std::shared_ptr<Tank> tank, const Wall &wall)
    {
        Size3D rotatedTankSize = GetRotatedSize(tank->GetSize(), tank->GetRotation());
        Size3D rotatedWallSize = GetRotatedSize(wall.GetSize(), wall.GetRotation());
        return CheckCollision(wall.GetPosition(), rotatedWallSize, tank->GetPosition(), rotatedTankSize);
    }

    static std::optional<CollisionData> Detect(std::shared_ptr<Tank> tank, const Headquarters &headquarters)
    {
        Size3D rotatedTankSize = GetRotatedSize(tank->GetSize(), tank->GetRotation());
        Size3D rotatedHeadquartersSize = GetRotatedSize(headquarters.GetSize(), headquarters.GetRotation());
        return CheckCollision(headquarters.GetPosition(), rotatedHeadquartersSize, tank->GetPosition(), rotatedTankSize);
    }

    static std::optional<CollisionData> Detect(std::shared_ptr<Tank> tank, const Bonus &bonus)
    {
        Size3D rotatedTankSize = GetRotatedSize(tank->GetSize(), tank->GetRotation());
        Size3D rotatedBonusSize = GetRotatedSize(bonus.GetSize(), bonus.GetRotation());
        return CheckCollision(bonus.GetPosition(), rotatedBonusSize, tank->GetPosition(), rotatedTankSize);
    }

private:
    struct Bound {
        float left;
        float right;
        float front;
        float back;
    };

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