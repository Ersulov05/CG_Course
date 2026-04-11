#pragma once
#include "../../common/Geometry.h"
#include "../../common/TransformMatrix.h"
#include "../Ball.h"
#include "../Block.h"
#include "../Racket.h" 
#include <optional>

class CollisionSystem
{
public:
    static bool CheckCollision(Ball &ball, const Block &block)
    {
        auto collisionData = CheckCollision(block.GetPosition(), block.GetSize(), ball.GetPosition(), ball.GetRadius());

        if (!collisionData.has_value()) {
            return false;
        }

        ChangeBallMoveDirection(ball, collisionData.value());
        return true;
    }

    static bool CheckCollision(Ball &ball, const Racket &racket)
    {
        auto collisionData = CheckCollision(racket.GetPosition(), racket.GetSize(), ball.GetPosition(), ball.GetRadius());

        if (!collisionData.has_value()) {
            return false;
        }

        ChangeBallMoveDirection(ball, collisionData.value());
        return true;
    }

private:
    struct CollisionData {
        Vector3D normal;
        float overlap;
    };

    static std::optional<CollisionData> CheckCollision(Point3D blockPos, Size3D blockSize, Point3D ballPos, float ballRadius)
    {
        float blockLeft = blockPos.x - blockSize.width / 2;
        float blockRight = blockPos.x + blockSize.width / 2;
        float blockFront = blockPos.z - blockSize.depth / 2;
        float blockBack = blockPos.z + blockSize.depth / 2;
        
        float closestX = std::max(blockLeft, std::min(ballPos.x, blockRight));
        float closestZ = std::max(blockFront, std::min(ballPos.z, blockBack));
        
        float dx = ballPos.x - closestX;
        float dz = ballPos.z - closestZ;
        float distance = sqrt(dx * dx + dz * dz);
        
        if (distance >= ballRadius) {
            return std::nullopt;
        }

        Vector3D normal = {dx / distance, 0, dz / distance};
        float overlap = ballRadius - distance;
        return CollisionData{normal, overlap};
    }

    static void ChangeBallMoveDirection(Ball& ball, CollisionData collisionData) {
        auto ballPos = ball.GetPosition();

        ball.SetPosition({
            ballPos.x + collisionData.normal.x * collisionData.overlap,
            ballPos.y,
            ballPos.z + collisionData.normal.z * collisionData.overlap
        });

        auto moveDirection = ball.GetMoveDirection();

        float dot = moveDirection.x * collisionData.normal.x + moveDirection.z * collisionData.normal.z;
        float newVx = moveDirection.x - 2 * dot * collisionData.normal.x;
        float newVz = moveDirection.z - 2 * dot * collisionData.normal.z;

        ball.SetMoveDirection({newVx, 0, newVz});
    }
};