#pragma once
#include <memory>
#include "../Tank/Tank.h"
#include <random>
#include <functional>
#include <unordered_map>
#include "../Shell/ShellManager.h"

class EnemyStrategy {
public:
    EnemyStrategy(ShellManager& shellManager)
        : m_shellManager(shellManager)
    {
    }

    void Update(float deltatime)
    {
        std::erase_if(m_enemyMap, [](auto& pair) { 
            auto& tank = pair.first;
            return !tank || tank->GetHealth() == 0; 
        });

        for (auto& pair : m_enemyMap) {
            EnemyStrategyData& strategyData = pair.second;
            Update(strategyData, deltatime);
        }
    }

    void ApplyStrategy(std::shared_ptr<Tank> enemy) {
        auto& enemyStrategyData = m_enemyMap[enemy];

        if (enemyStrategyData.rotationTimer <= 0 && GetRandomByPercent(CHANGE_ROTATION_PERCENT)) {
            ChangeRotation(enemy, enemyStrategyData);
        }
        if (enemyStrategyData.moveTimer <= 0 && GetRandomByPercent(CHANGE_MOVE_PERCENT)) {
            ChangeMove(enemy, enemyStrategyData);
        }
        if (enemyStrategyData.fireTimer <= 0 && GetRandomByPercent(FIRE_PERCENT)) {
            m_shellManager.AddShells(enemy->Fire());
            enemyStrategyData.fireTimer = 1;
        }
    }
private:
    ShellManager& m_shellManager;
    inline static std::mt19937 m_randomEngine = std::mt19937(std::random_device{}());
    inline static const float CHANGE_ROTATION_PERCENT = 5;
    inline static const float CHANGE_MOVE_PERCENT = 20;
    inline static const float FIRE_PERCENT = 50;

    struct EnemyStrategyData {
        float rotationTimer = 0;
        float fireTimer = 0;
        float moveTimer = 0;
    };

    std::unordered_map<std::shared_ptr<Tank>, EnemyStrategyData> m_enemyMap;

    static void Update(EnemyStrategyData& enemyStrategyData, float deltatime) 
    {
        enemyStrategyData.fireTimer = std::max(0.0f, enemyStrategyData.fireTimer - deltatime);
        enemyStrategyData.rotationTimer = std::max(0.0f, enemyStrategyData.rotationTimer - deltatime);
        enemyStrategyData.moveTimer = std::max(0.0f, enemyStrategyData.moveTimer - deltatime);
    }

    static bool GetRandomByPercent(float percent) 
    {
        std::uniform_real_distribution<float> dist(0.0f, 100.0f);

        float randomPercent = dist(m_randomEngine);
        return randomPercent < percent;
    }

    static void ChangeRotation(std::shared_ptr<Tank> enemy, EnemyStrategyData& enemyStrategyData) 
    {
        std::uniform_int_distribution<int> dist(0, 3);
        int randomDirection = dist(m_randomEngine);

        enemy->Rotate((Direction)randomDirection);
        enemyStrategyData.rotationTimer = 1.5;
    }

    static void ChangeMove(std::shared_ptr<Tank> enemy, EnemyStrategyData& enemyStrategyData) 
    {
        std::uniform_int_distribution<int> dist(0, 1);
        int randomMove = dist(m_randomEngine);

        if (randomMove) {
            enemy->Stop();
        } else {
            enemy->Move();
        }

        enemyStrategyData.moveTimer = 2;
    }
};