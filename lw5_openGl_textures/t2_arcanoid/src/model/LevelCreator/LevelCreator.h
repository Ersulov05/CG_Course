#pragma once
#include "../Constants.h"
#include "../Block.h"
#include "../Ball.h"
#include <random>
#include <functional>
#include <memory>

class LevelCreator
{
public:
    static std::vector<Block> GetLevelBlocks(unsigned level) {
        std::vector<Block> levelBlocks;
        auto levelData = LEVELS[(level - 1) % LEVELS.size()]; 

        float startX = -(float)levelData.size() / 2 * BLOCK_SIZE.width + BLOCK_SIZE.width/2;
        float z = -SCENE_SIZE.depth + BLOCK_SIZE.depth/2;

        float x = startX;
        for (auto& levelRow : levelData) {
            for (auto& blockItem : levelRow) {
                if (blockItem >= '1' && blockItem <= '7') {
                    levelBlocks.push_back(Block({x, 0, z}, BLOCK_SIZE, GetColoredBlockMesh(blockItem)));
                }
                x += BLOCK_SIZE.width;
            }
            x = startX;
            z += BLOCK_SIZE.depth;
        }

        return levelBlocks;
    }

    static Ball GetLevelStartBall() {
        auto moveDirection = GetRandomMoveDirectionVector();

        return Ball(DEFAULT_BALL_POSITION, moveDirection, DEFAULT_BALL_RADIUS, DEFAULT_BALL_SPEED);
    }
private:
    inline static std::mt19937 m_randomEngine{std::random_device{}()};

    static Vector3D GetRandomMoveDirectionVector() {
        std::uniform_real_distribution<float> distAngle(MIN_ANGLE, MAX_ANGLE);
        std::uniform_int_distribution<int> distAngleCoef(0, 1);

        int angleCoef = distAngleCoef(m_randomEngine) ?: -1;
        float randomAngle = distAngle(m_randomEngine) * angleCoef + 90;
        float rad = randomAngle * M_PI / 180.0f;

        return Vector3D(cos(rad), 0, -sin(rad));
    }

    inline static const float MIN_ANGLE = 10;
    inline static const float MAX_ANGLE = 60;

    using LevelBlocksMap = std::vector<std::string>;

    // 0 - EMPTY
    inline static const std::unordered_map<char, BlockColorType> CHAR_TO_COLOR_TYPE = {
        {'1', BlockColorType::RED},
        {'2', BlockColorType::GREEN},
        {'3', BlockColorType::LIGHT_BLUE},
        {'4', BlockColorType::DARK_BLUE},
        {'5', BlockColorType::ORANGE},
        {'6', BlockColorType::YELLOW},
        {'7', BlockColorType::VIOLET},
    };

    inline static const std::vector<LevelBlocksMap> LEVELS = {
        {
            "1770000771",
            "0611111160",
            "0122332210",
            "0724554270",
            "0122332210",
            "1611111161",
            "4005555004",
            "6000440006",
            "0000000000",
            "0000000000",
        },
        {
            "0000000000",
            "0611111160",
            "0120000210",
            "0720550270",
            "0120330210",
            "0600110060",
            "0202002020",
            "0003003000",
            "0040000400",
            "0000000000",
        },
    };

    static BlockColorType GetColoredBlockMesh(char ch) {
        auto it = CHAR_TO_COLOR_TYPE.find(ch);
        if (it != CHAR_TO_COLOR_TYPE.end()) {
            return it->second;
        }
        
        return BlockColorType::GREEN;
    };
};