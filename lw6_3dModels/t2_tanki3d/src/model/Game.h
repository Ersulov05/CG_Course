#pragma once
#include "./Tank/Tank.h"
#include <vector>
#include "./Shell/Shell.h"
#include "./Map/Map.h" 
#include "./LevelCreator/LevelCreator.h"
#include "./Shell/ShellManager.h"
#include "./Enemy/EnemyManager.h"
#include "./Collision/CollisionHandler.h"

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

    const std::vector<std::shared_ptr<Tank>>& GetEnemyTanks() const 
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
        CollisionHandler::CheckAndHandleCollision(m_playerTank, m_map);
        for (auto& tank : m_enemyManager.GetEnemies()) {
            CollisionHandler::CheckAndHandleCollision(tank, m_map);
            CollisionHandler::CheckAndHandleCollision(tank, m_map);
            for (auto & shell : m_shellManager.GetShells())
            {
                CollisionHandler::CheckAndHandleCollision(shell, tank);
            }
        }
        for (auto & shell : m_shellManager.GetShells())
        {
            CollisionHandler::CheckAndHandleCollision(shell, m_playerTank);
            CollisionHandler::CheckAndHandleCollision(shell, m_map);
        }
    }
};