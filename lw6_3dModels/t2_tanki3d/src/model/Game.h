#pragma once
#include "./Tank/Tank.h"
#include <vector>
#include "./Shell/Shell.h"
#include "./Map/Map.h" 
#include "./LevelCreator/LevelCreator.h"
#include "./Shell/ShellManager.h"
#include "./Collision/CollisionSystem.h"

class Game {
public:
    void Update(float deltatime) 
    {
        m_playerTank.Update(deltatime);
        m_shellManager.Update(deltatime);
        m_map.Update();

        for (auto& wall: m_map.GetWalls()) {
            CollisionSystem::CheckAndHandleCollision(m_playerTank, wall);
            for (auto & shell : m_shellManager.GetShells())
            {
                if (CollisionSystem::CheckAndHandleCollision(shell, wall))
                {
                    shell.Boom();
                    wall.TakeDamage();
                };
            }
        }
    }

    Tank& GetPlayerTank() 
    {
        return m_playerTank;
    }

    const Tank& GetPlayerTank() const 
    {
        return m_playerTank;
    }

    const std::vector<Tank>& GetEnemyTanks() const 
    {
        return m_enemyTanks;
    }

    const Map& GetMap() const 
    {
        return m_map;
    }

    ShellManager& GetShellManager()
    {
        return m_shellManager;
    }

    const ShellManager& GetShellManager() const
    {
        return m_shellManager;
    }
private:
    Map m_map = LevelCreator::GetMapByLevel(1);
    ShellManager m_shellManager;
    Tank m_playerTank = Tank(m_map, {0, 0, 0}, 1);
    std::vector<Tank> m_enemyTanks;
};