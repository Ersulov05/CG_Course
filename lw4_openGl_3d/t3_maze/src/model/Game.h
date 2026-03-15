#pragma once
#include "./Maze.h"
#include "./Player.h"

class Game
{
public:
    void Update() {

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
};