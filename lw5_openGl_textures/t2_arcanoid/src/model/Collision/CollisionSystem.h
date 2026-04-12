#pragma once
#include "../../common/Geometry.h"
#include "../../common/TransformMatrix.h"
#include "../Ball.h"
#include "../Block.h"
#include "../Racket.h" 
#include "../Bonus/Bonus.h"
#include "../Constants.h"
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

    static bool CheckCollision(Bonus &bonus, const Racket &racket)
    {
        auto collisionData = CheckCollision(racket.GetPosition(), racket.GetSize(), bonus.GetPosition(), bonus.GetRadius());

        if (!collisionData.has_value()) {
            return false;
        }

        return true;
    }

    static void CheckAndHandleBallWithSceneCollision(Ball &ball) {
        auto ballPos = ball.GetPosition();
        auto ballRadius = ball.GetRadius();

        auto rightDistance = SCENE_SIZE.width/2 - ballPos.x - ballRadius;
        auto leftDistance = ballPos.x - (-SCENE_SIZE.width/2) - ballRadius;
        auto upDistance = ballPos.z - (-SCENE_SIZE.depth) - ballRadius;

        auto newPos = ball.GetPosition();
        auto newDirection = ball.GetMoveDirection();
        if (rightDistance < 0) {
            newDirection.x *= -1;
            newPos.x += rightDistance;
        }

        if (leftDistance < 0) {
            newDirection.x *= -1;
            newPos.x -= leftDistance;
        }

        if (upDistance < 0) {
            newDirection.z *= -1;
            newPos.z -= upDistance;
        }

        ball.SetMoveDirection(newDirection);
        ball.SetPosition(newPos);
    }

    static void CheckAndHandleRacketWithSceneCollision(Racket& racket) {
        auto racketPosX = racket.GetPosition().x;
        auto racketWidth = racket.GetSize().width;

        auto rightDistance = SCENE_SIZE.width/2 - racketPosX - racketWidth / 2;
        auto leftDistance = racketPosX - (-SCENE_SIZE.width/2) - racketWidth / 2;

        auto newPos = racket.GetPosition();
        if (rightDistance < 0) {
            newPos.x += rightDistance;
        }

        if (leftDistance < 0) {
            newPos.x -= leftDistance;
        }

        racket.SetPosition(newPos);
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
        
        if (distance >= ballRadius || distance == 0) {
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