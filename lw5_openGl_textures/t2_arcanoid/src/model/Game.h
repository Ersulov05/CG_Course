#pragma once
#include "./Ball.h"
#include "./Racket.h"
#include "./Block.h"
#include "./Collision/CollisionSystem.h"
#include "./Bonus/BonusActionManager.h"
#include "./Bonus/Bonus.h"
#include "./Bonus/SplitBallsBonusAction.h"
#include "./Bonus/AddSpeedBallsBonusAction.h"
#include "./Bonus/SubSizeBallsBonusAction.h"
#include "./Bonus/Factory/BonusFactory.h"

#include "./Constants.h"
#include "./LevelCreator/LevelCreator.h"

class Game
{
public:
    Game() : m_bonusFactory(m_balls, m_racket) {
        m_balls.push_back(LevelCreator::GetLevelStartBall());
        m_blocks = LevelCreator::GetLevelBlocks(m_level);
    }

    Racket& GetRacket() {
        return m_racket;
    }

    std::vector<Block>& GetBlocks() {
        return m_blocks;
    }

    std::vector<Ball>& GetBalls() {
        return m_balls;
    }

    std::vector<Bonus>& GetBonuses() {
        return m_bonuses;
    }

    void Update(float deltatime) {
        for (auto &ball : m_balls) {
            ball.Update(deltatime);
        }

        for (auto &bonus : m_bonuses) {
            bonus.Update(deltatime);
        }
        m_bonusManager.Update(deltatime);

        CheckAndHandleCollision();
        DeleteOutBalls();

        if (m_blocks.size() == 0) {
            ++m_level;
            ResetLevel();
        }
        if (m_balls.size() == 0) {
            ResetLevel();
        }
    }
private:
    Racket m_racket;
    std::vector<Ball> m_balls;
    std::vector<Block> m_blocks;
    std::vector<Bonus> m_bonuses;
    BonusActionManager m_bonusManager;
    BonusFactory m_bonusFactory;
    int m_brokenBlocksCount = 0;
    unsigned m_level = 1;
    const int BLOCKS_TO_SPAWN_BONUS = 4;
    const float OUT_POSITION_Z = 0.5;

    void Reset() {
        m_bonusManager.Clear();
        m_balls.clear();
        m_bonuses.clear();
        m_racket.Reset();
        m_brokenBlocksCount = 0;
    }

    void ResetLevel() {
        Reset();
        m_blocks = LevelCreator::GetLevelBlocks(m_level);
        m_balls.push_back(LevelCreator::GetLevelStartBall());
    }

    void CheckAndHandleCollision() {
        CollisionSystem::CheckAndHandleRacketWithSceneCollision(m_racket);
        m_bonuses.erase(
            std::remove_if(m_bonuses.begin(), m_bonuses.end(),
            [this](Bonus& bonus) {
                if (CollisionSystem::CheckCollision(bonus, m_racket)) {
                    m_bonusManager.ApplyBonus(bonus.GetBonusAction());
                    return true;
                }
                return false;
            }),
            m_bonuses.end()
        );

        for (auto& ball : m_balls) {
            m_blocks.erase(
                std::remove_if(m_blocks.begin(), m_blocks.end(),
                    [&ball, this](Block& block) {
                        if (CollisionSystem::CheckCollision(ball, block)) {
                            HandleBlockCollision(block);
                            return true;
                        }
                        return false;
                    }
                ),
                m_blocks.end()
            );

            CollisionSystem::CheckCollision(ball, m_racket);
            CollisionSystem::CheckAndHandleBallWithSceneCollision(ball);
        }
    }

    void HandleBlockCollision(const Block& block) 
    {
        ++m_brokenBlocksCount;
        if (m_brokenBlocksCount != 0 && m_brokenBlocksCount % BLOCKS_TO_SPAWN_BONUS == 0) 
        {
            auto newBonus = m_bonusFactory.CreateBonus(block.GetPosition());
            m_bonuses.push_back(newBonus);
        }        
    }

    void DeleteOutBalls() {
        m_balls.erase(
            std::remove_if(m_balls.begin(), m_balls.end(),
                [this](const Ball& ball) {
                    return ball.GetPosition().z > OUT_POSITION_Z;
                }
            ),
            m_balls.end()
        );  
    }

    void DeleteOutBonuses() {
        m_bonuses.erase(
            std::remove_if(m_bonuses.begin(), m_bonuses.end(),
                [this](const Bonus& bonus) {
                    return bonus.GetPosition().z > OUT_POSITION_Z;
                }
            ),
            m_bonuses.end()
        );  
    }
};