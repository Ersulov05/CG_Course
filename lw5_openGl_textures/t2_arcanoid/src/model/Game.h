#pragma once
#include "./Ball.h"
#include "./Racket.h"
#include "./Block.h"
#include "./Collision/CollisionSystem.h"

class Game
{
public:
    inline static const Size3D SCENE_SIZE = Size3D{2, 1, 3};

    Game() {
        m_balls.push_back(Ball({0, 0, -1}, {-1, 0, 1}, 0.05, 0.5));
        m_balls.push_back(Ball({0, 0, -1}, {1, 0, -1}, 0.05, 0.5));

        m_blocks.push_back(Block({0, 0, -3}, {0.4, 0.2, 0.2}));
        m_blocks.push_back(Block({0, 0, -2.5}, {0.4, 0.2, 0.2}));
        m_blocks.push_back(Block({0, 0, -2}, {0.4, 0.2, 0.2}));
    }

    Racket& GetRacket() {
        return m_racket;
    }

    std::vector<Block>& GetBlocks() {
        return m_blocks;
    }

    std::vector<Ball>& GetBalls() {
        return m_balls;
    }

    void Update(float deltatime) {
        for (auto &ball : m_balls) {
            ball.Update(deltatime);
        }

        CheckCollision();
        CheckRacketWithSceneCollision();
        DeleteOutBalls();
    }
private:
    Racket m_racket;
    std::vector<Ball> m_balls;
    std::vector<Block> m_blocks;

    void CheckCollision() {
        for (auto& ball : m_balls) {
            m_blocks.erase(
                std::remove_if(m_blocks.begin(), m_blocks.end(),
                    [&ball, this](Block& block) {
                        if (CollisionSystem::CheckCollision(ball, block)) {
                            HandleBlockCollision(block);
                            return true;
                        }
                        return false;
                    }
                ),
                m_blocks.end()
            );

            CollisionSystem::CheckCollision(ball, m_racket);
            CheckBallWithSceneCollision(ball);
        }
    }

    void HandleBlockCollision(const Block& block) {

    }

    void CheckBallWithSceneCollision(Ball &ball) {
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
        ballPos = ball.GetPosition();
    }

    void DeleteOutBalls() {
        m_balls.erase(
            std::remove_if(m_balls.begin(), m_balls.end(),
                [](const Ball& ball) {
                    return ball.GetPosition().z > 0;
                }
            ),
            m_balls.end()
        );  
    }

    void CheckRacketWithSceneCollision() {
        auto racketPosX = m_racket.GetPosition().x;
        auto racketWidth = m_racket.GetSize().width;

        auto rightDistance = SCENE_SIZE.width/2 - racketPosX - racketWidth / 2;
        auto leftDistance = racketPosX - (-SCENE_SIZE.width/2) - racketWidth / 2;

        auto newPos = m_racket.GetPosition();
        if (rightDistance < 0) {
            newPos.x += rightDistance;
        }

        if (leftDistance < 0) {
            newPos.x -= leftDistance;
        }

        m_racket.SetPosition(newPos);
    }
};