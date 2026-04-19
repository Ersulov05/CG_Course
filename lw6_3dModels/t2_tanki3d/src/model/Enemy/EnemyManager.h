#pragma once
#include <vector>
#include "../Tank/Tank.h"
#include "../Map/Map.h"
#include <random>
#include "../Constants.h"
#include "../Collision/CollisionDetector.h"
#include <functional>

class EnemyManager {
public:
    EnemyManager(const Map& map, const std::shared_ptr<Tank> player)
        : m_map(map)
        , m_player(player)
    {
    }

    void Update(float deltatime) 
    {
        if (m_enemies.size() < MAX_ENEMIES) {
            SpawnEnemy();
        }

        std::erase_if(m_enemies, [](const std::shared_ptr<Tank>& tank) { 
            return !tank || tank->GetHealth() == 0; 
        });
    }

    const std::vector<std::shared_ptr<Tank>>& GetEnemies() const
    {
        return m_enemies;
    }

    std::vector<std::shared_ptr<Tank>>& GetEnemies() 
    {
        return m_enemies;
    }

private:
    std::vector<std::shared_ptr<Tank>> m_enemies;

    const Map& m_map;
    const std::shared_ptr<Tank> m_player;
    const int MAX_ENEMIES = 1;
    float MIN_DISTANCE_TO_PLAYER2 = 30 * 30;

    std::mt19937 m_randomEngine = std::mt19937(std::random_device{}());;

    void SpawnEnemy()
    {
        float mapWidth = m_map.GetWidth() - Constants::TERRARIAN_SIZE.width;
        float mapHeight = m_map.GetHeight() - Constants::TERRARIAN_SIZE.depth;

        std::uniform_real_distribution<float> distX(-mapWidth/2, mapWidth/2);
        std::uniform_real_distribution<float> distZ(-mapHeight/2, mapHeight/2);

        Point3D randomPos = {distX(m_randomEngine), 0, distZ(m_randomEngine)};
        Vector3D distance = randomPos - m_player->GetPosition();
        if (MIN_DISTANCE_TO_PLAYER2 < distance.x * distance.x + distance.z * distance.z)
        {
            return;
        }

        auto enemyTank = std::make_shared<Tank>(m_map, TankType::T34, 1, randomPos);
        if (CheckCollisionSpawnedTank(enemyTank))
        {
            return;
        }

        m_enemies.push_back(enemyTank);
    }

    bool CheckCollisionSpawnedTank(const std::shared_ptr<Tank> tank)
    {
        for (auto& wall : m_map.GetWalls()) {
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