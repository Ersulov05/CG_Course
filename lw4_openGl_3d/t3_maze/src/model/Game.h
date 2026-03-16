#pragma once
#include "./Maze.h"
#include "./Player.h"
#include "./Collision/CollisionSystem.h"

class Game
{
public:
    Game() {
        m_player.OnChangeCollisionSubscribe(this, [this](){
            return CheckCollisions();
        });
    }

    Maze& GetMaze() {
        return m_maze;
    }

    Player& GetPlayer() {
        return m_player;
    }
private:
    Maze m_maze;
    Player m_player;

    bool CheckCollisions() {
        auto walls = m_maze.GetWalls();
        auto playerCollisions = m_player.GetCollisions();

        for (auto & wall : walls) {
            auto wallCollision = wall.GetCollision();
            for (auto playerCollision : playerCollisions) {
                if (CollisionSystem::CheckCollision(playerCollision, wallCollision)) {
                    return true;
                }
            }
        }

        return false;
    }
};