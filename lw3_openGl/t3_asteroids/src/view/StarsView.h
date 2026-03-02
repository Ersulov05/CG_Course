#pragma once
#include "../common/Point.h"
#include "../common/Color.h"
#include "./Canvas/ICanvas.h"
#include <vector>
#include <random>

class StarsView
{
public:
    StarsView()
    {
        GenerateStars();
    }

    void Draw(ICanvas &canvas, const Point &playerPos)
    {
        UpdateStars(playerPos);
        DrawStars(canvas);
    }

private:
    struct Star
    {
        Point position;
        float size;
        Color color;
    };

    std::vector<Star> m_stars;
    const int STAR_COUNT = 100;
    const float MAX_STARS_DISTANCE = 500;

    void GenerateStars()
    {
        m_stars.clear();
        m_stars.reserve(STAR_COUNT);

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> distPos(-MAX_STARS_DISTANCE, MAX_STARS_DISTANCE);
        std::uniform_real_distribution<float> distBrightness(0.3f, 1.0f);
        std::uniform_real_distribution<float> distSize(1.0f, 3.0f);

        for (int i = 0; i < STAR_COUNT; ++i)
        {
            Color starColor = Color(0xFFFFFFFF);
            starColor.a = distBrightness(gen);

            Star star;
            star.position.x = distPos(gen);
            star.position.y = distPos(gen);
            star.size = distSize(gen);
            star.color = starColor;
            m_stars.push_back(star);
        }
    }

    void UpdateStars(const Point &playerPos)
    {
        for (auto &star : m_stars)
        {
            Point relativePos = star.position - playerPos;

            if (relativePos.x > MAX_STARS_DISTANCE)
            {
                star.position.x = playerPos.x - MAX_STARS_DISTANCE + (relativePos.x - MAX_STARS_DISTANCE);
            }
            else if (relativePos.x < -MAX_STARS_DISTANCE)
            {
                star.position.x = playerPos.x + MAX_STARS_DISTANCE + (relativePos.x + MAX_STARS_DISTANCE);
            }

            if (relativePos.y > MAX_STARS_DISTANCE)
            {
                star.position.y = playerPos.y - MAX_STARS_DISTANCE + (relativePos.y - MAX_STARS_DISTANCE);
            }
            else if (relativePos.y < -MAX_STARS_DISTANCE)
            {
                star.position.y = playerPos.y + MAX_STARS_DISTANCE + (relativePos.y + MAX_STARS_DISTANCE);
            }
        }
    }

    void DrawStars(ICanvas &canvas)
    {
        for (auto &star : m_stars)
        {
            DrawStar(canvas, star);
        }
    }

    void DrawStar(ICanvas &canvas, const Star &star)
    {
        canvas.SetColor(star.color);
        canvas.FillRect({star.position.x - star.size / 2, star.position.y - star.size / 2}, {star.size, star.size});
    }
};