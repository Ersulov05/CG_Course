#pragma once

#include <vector>
#include "./Wall.h"

class Maze
{
public:
    Maze() {
        GenerateMaze();
    }

    const std::vector<Wall>& GetWalls() const {
        return m_walls;
    }

private:
    std::vector<Wall> m_walls;

    void GenerateMaze() {
        m_walls.clear();

        m_walls.push_back(Wall({0, 1.5, -8}, {16, 3, 1}, 0xFFFFFFFF));
        m_walls.push_back(Wall({0, 1.5, 8}, {16, 3, 1}, 0xFFFFFFFF));
        m_walls.push_back(Wall({-8, 1.5, 0}, {1, 3, 16}, 0x00FF00FF));
        m_walls.push_back(Wall({8, 1.5, 0}, {1, 3, 16}, 0x00FF00FF));

        m_walls.push_back(Wall({-4, 1.5, -2}, {0.5, 3, 4.5}, 0x00FFFFFF));
        m_walls.push_back(Wall({0, 1.5, -2}, {0.5, 3, 4.5}, 0x00FFFFFF));
        m_walls.push_back(Wall({2, 1.5, -4}, {4, 3, 0.5}, 0x00FFFFFF));
        m_walls.push_back(Wall({4, 1.5, -2}, {0.5, 3, 4.5}, 0x00FFFFFF));
        m_walls.push_back(Wall({6, 1.5, 0}, {3.5, 3, 0.5}, 0x00FFFFFF));
        m_walls.push_back(Wall({0, 1.5, 4}, {8.5, 3, 0.5}, 0x00FFFFFF));

        m_walls.push_back(Wall({-2, 1.5, 0}, {3.499, 3, 0.5}, 0x0000FF2F));
        m_walls.push_back(Wall({4, 1.5, 2}, {0.5, 3, 3.499}, 0x0000AF2F));

        m_walls.push_back(Wall({0, -0.25, 0}, {16, 0.5, 16}, 0x9F9F00FF));
        m_walls.push_back(Wall({0, 3.25, 0}, {16, 0.5, 16}, 0x009F9FFF));


    }
};