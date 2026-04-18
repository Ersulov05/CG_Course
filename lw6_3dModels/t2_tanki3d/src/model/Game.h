#pragma once
#include "./Tank/Tank.h"
#include <vector>
#include "./Shell/Shell.h"
#include "./Map/Map.h" 
#include "./LevelCreator/LevelCreator.h"
#include "./Shell/ShellManager.h"
#include "./Collision/CollisionSystem.h"
#include "./Enemy/EnemyManager.h"

class Game {
public:
    Game() 
        : m_map(LevelCreator::GetMapByLevel(1))
        , m_playerTank(std::make_shared<Tank>(m_map, TankType::T34, 2))
        , m_enemyManager(m_map, m_playerTank)
    {
    }

    void Update(float deltatime) 
    {
        m_playerTank->Update(deltatime);
        m_shellManager.Update(deltatime);
        m_enemyManager.Update(deltatime);
        m_map.Update();
        CheckAndHandleCollisions();
    }

    std::shared_ptr<Tank> GetPlayerTank() 
    {
        return m_playerTank;
    }

    const std::shared_ptr<Tank> GetPlayerTank() const 
    {
        return m_playerTank;
    }

    const std::vector<std::shared_ptr<Tank>> GetEnemyTanks() const 
    {
        return m_enemyManager.GetEnemies();
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
    Map m_map;
    ShellManager m_shellManager;
    std::shared_ptr<Tank> m_playerTank;
    EnemyManager m_enemyManager;

    void CheckAndHandleCollisions()
    {
        for (auto& wall: m_map.GetWalls()) {
            for (auto & shell : m_shellManager.GetShells())
            {
                CollisionSystem::CheckAndHandleCollision(shell, wall);
            }
            CollisionSystem::CheckAndHandleCollision(m_playerTank, wall);
        }
        for (auto & shell : m_shellManager.GetShells())
        {
            for (auto& tank : m_enemyManager.GetEnemies()) {
                CollisionSystem::CheckAndHandleCollision(shell, tank);
            }
            CollisionSystem::CheckAndHandleCollision(shell, m_playerTank);
        }
    }
};