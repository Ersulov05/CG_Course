#pragma once

#include "../Tank/Tank.h"
#include "../Map/Map.h"
#include "../Constants.h"
#include "../Collision/CollisionDetector.h"
#include "./EnemyStrategy.h"
#include <functional>
#include <random>
#include <vector>

class EnemyManager {
public:
    EnemyManager(
        const Map& map, 
        const std::shared_ptr<Tank>& player,
        ShellManager& shellManager
    )
        : m_map(map)
        , m_player(player)
        , m_enemyStrategy(shellManager)
    {
    }

    void Update(float deltatime) 
    {
        if (m_enemies.size() < Constants::MAX_ENEMIES) {
            if (m_enemyRespawnTime > 0) {
                m_enemyRespawnTime -= deltatime;
            } else {
                SpawnEnemy();
            }
        }

        std::erase_if(m_enemies, [this](const std::shared_ptr<Tank>& tank) {
            if (!tank || tank->GetHealth() == 0) {
                ++m_killCount;
                return true;
            }
            return false; 
        });

        m_enemyStrategy.Update(deltatime);
        for (auto& enemy : m_enemies) {
            m_enemyStrategy.ApplyStrategy(enemy);
            enemy->Update(deltatime);
        }
    }

    const std::vector<std::shared_ptr<Tank>>& GetEnemies() const
    {
        return m_enemies;
    }

    std::vector<std::shared_ptr<Tank>>& GetEnemies() 
    {
        return m_enemies;
    }

    void Clear()
    {
        m_enemyStrategy.Clear();
        m_enemies.clear();
        m_killCount = 0;
        m_enemyRespawnTime = 0;
    }

    unsigned int GetKillCount() const
    {
        return m_killCount;
    }

private:
    std::vector<std::shared_ptr<Tank>> m_enemies;
    EnemyStrategy m_enemyStrategy;
    unsigned m_killCount = 0;
    float m_enemyRespawnTime = 0;

    const Map& m_map;
    const std::shared_ptr<Tank>& m_player;
    float MIN_DISTANCE_TO_PLAYER_SQ = 12 * 12;

    std::mt19937 m_randomEngine = std::mt19937(std::random_device{}());;

    void SpawnEnemy()
    {
        float mapWidth = m_map.GetWidth() - Constants::TERRARIAN_SIZE.width;
        float mapHeight = m_map.GetHeight() - Constants::TERRARIAN_SIZE.depth;

        std::uniform_real_distribution<float> distX(-mapWidth/2, mapWidth/2);
        std::uniform_real_distribution<float> distZ(-mapHeight/2, mapHeight/2);
        std::uniform_int_distribution<unsigned int> distLevel(1, 4);

        Point3D randomPos = {distX(m_randomEngine), 0, distZ(m_randomEngine)};
        unsigned int randomLevel = distLevel(m_randomEngine);
        Vector3D distance = randomPos - m_player->GetPosition();
        if (distance.x * distance.x + distance.z * distance.z < MIN_DISTANCE_TO_PLAYER_SQ)
        {
            return;
        }

        auto enemyTank = std::make_shared<Tank>(m_map, TankType::TIGER, randomLevel, randomPos);
        if (CheckCollisionSpawnedTank(enemyTank))
        {
            return;
        }

        m_enemyRespawnTime = Constants::ENEMY_RESPAWN_TIME;
        m_enemies.push_back(enemyTank);
    }

    bool CheckCollisionSpawnedTank(const std::shared_ptr<Tank> tank)
    {
        for (auto& wall : m_map.GetWalls()) {
            if (CollisionDetector::Detect(tank, wall).has_value()) {
                return true;
            }
        }
        for (auto& wall : m_map.GetHeadquartersWalls()) {
            if (CollisionDetector::Detect(tank, wall).has_value()) {
                return true;
            }
        }
        if (CollisionDetector::Detect(tank, m_map.GetHeadquarters()).has_value())
        {
            return true;
        }

        return false;
    }
};