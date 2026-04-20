#pragma once
#include "./Tank/Tank.h"
#include <vector>
#include "./Shell/Shell.h"
#include "./Map/Map.h" 
#include "./LevelCreator/LevelCreator.h"
#include "./Shell/ShellManager.h"
#include "./Enemy/EnemyManager.h"
#include "./Effect/EffectManager.h"
#include "./Collision/CollisionHandler.h"
#include "./Collision/CollisionDetector.h"

#include "./Bonus/BonusManager.h"
#include "./Bonus/BonusActionManager.h"
#include "./Bonus/BonusAction/Factory/BonusActionFactory.h"
#include "./Effect/EffectFactory.h"

enum class GameState {
    Win,
    Over,
    Play
};

class Game {
public:
    Game() 
        : m_map(LevelCreator::GetMapByLevel(1))
        , m_playerTank(std::make_shared<Tank>(m_map, TankType::T34, 1, Constants::PLAYER_POS))
        , m_shellManager(ShellManager())
        , m_enemyManager(m_map, m_playerTank, m_shellManager)
        , m_bonusManager(m_map)
        , m_bonusActionFactory(m_map, m_enemyManager.GetEnemies())
        , m_state(GameState::Play)
    {
    }

    void Update(float deltatime) 
    {
        if (m_state != GameState::Play) {
            if (m_reloadTime > 0) {
                m_reloadTime -= deltatime;
                return;
            }
            Restart();
        }
        m_playerTank->Update(deltatime);
        m_shellManager.Update(deltatime);
        m_enemyManager.Update(deltatime);
        m_bonusManager.Update(deltatime);
        m_bonusActionManager.Update(deltatime);
        m_effectManager.Update(deltatime);
        m_map.Update();
        CheckAndHandleCollisions();
        UpdateGameState();
    }

    std::shared_ptr<Tank> GetPlayerTank() 
    {
        return m_playerTank;
    }

    const std::shared_ptr<Tank> GetPlayerTank() const 
    {
        return m_playerTank;
    }
    const EnemyManager& GetEnemyManager() const
    {
        return m_enemyManager;
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

    const BonusManager& GetBonusManager() const
    {
        return m_bonusManager;
    }

    const EffectManager& GetEffectManager() const
    {
        return m_effectManager;
    }

    GameState GetGameState() const
    {
        return m_state;
    }

    float GetReloadTime() const
    {
        return m_reloadTime;
    }

    inline static const float TOTAL_RELOAD_TIME = 3;
    inline static const float WIN_KILL_COUNT = 20;

private:
    Map m_map;
    ShellManager m_shellManager;
    std::shared_ptr<Tank> m_playerTank;
    EnemyManager m_enemyManager;
    BonusManager m_bonusManager;
    BonusActionManager m_bonusActionManager;
    BonusActionFactory m_bonusActionFactory;
    EffectManager m_effectManager;
    unsigned int m_level = 1;
    GameState m_state;
    float m_reloadTime = 0;

    void UpdateGameState() 
    {
        if (m_enemyManager.GetKillCount() >= WIN_KILL_COUNT) {
            m_state = GameState::Win;
            m_reloadTime = TOTAL_RELOAD_TIME;
        }
        if (
            m_map.GetHeadquarters().GetHealth() == 0 ||
            m_playerTank->GetHealth() == 0
        ) {
            m_state = GameState::Over;
            m_reloadTime = TOTAL_RELOAD_TIME;
        }
    }

    void Restart() {
        m_bonusActionManager.Clear();
        m_effectManager.Clear();
        m_bonusManager.Clear();
        m_enemyManager.Clear();
        m_shellManager.Clear();
        m_map = LevelCreator::GetMapByLevel(m_level);
        m_playerTank = std::make_shared<Tank>(m_map, TankType::T34, 1, Constants::PLAYER_POS);
        m_state = GameState::Play;
    }

    void CheckAndHandleCollisions()
    {
        CollisionHandler::CheckAndHandleCollision(m_playerTank, m_map);
        for (auto& tank : m_enemyManager.GetEnemies()) 
        {
            CollisionHandler::CheckAndHandleCollision(tank, m_map);
            for (auto & shell : m_shellManager.GetShells())
            {
                if (CollisionHandler::CheckAndHandleCollision(shell, tank))
                {
                    m_effectManager.AddEffect(EffectFactory::CreateEffect(EffectType::Boom, shell.GetPosition()));
                }
            }
        }
        for (auto & shell : m_shellManager.GetShells())
        {
            if (
                CollisionHandler::CheckAndHandleCollision(shell, m_playerTank) ||
                CollisionHandler::CheckAndHandleCollision(shell, m_map)
            )
            {
                m_effectManager.AddEffect(EffectFactory::CreateEffect(EffectType::Boom, shell.GetPosition()));
            }
        }

        for (auto & bonus : m_bonusManager.GetBonuses())
        {
            CheckAndHandleCollision(m_playerTank, bonus);
            if (IsOnlyPlayerBonusAction(bonus.GetType()))
            {
                continue;
            }

            for (auto& tank : m_enemyManager.GetEnemies()) 
            {
                CheckAndHandleCollision(tank, bonus);
            }
        }
        for (auto& firstTank : m_enemyManager.GetEnemies())
        {
            for (auto& secondTank : m_enemyManager.GetEnemies())
            {
                CollisionHandler::CheckAndHandleCollision(firstTank, secondTank);
            }
            CollisionHandler::CheckAndHandleCollision(m_playerTank, firstTank);
        }
    }

    void CheckAndHandleCollision(std::shared_ptr<Tank> tank, Bonus& bonus)
    {
        if(CollisionDetector::Detect(tank, bonus))
        {
            auto bonusActionType = bonus.GetType();
            auto bonusAction = m_bonusActionFactory.CreateBonusAction(bonusActionType, tank);
            m_bonusActionManager.ApplyBonus(bonusAction);
            bonus.Boom();
        }
    }
};