#pragma once
#include "../model/AsteroidModel.h"
#include "./Canvas/ICanvas.h"
#include <random>

class AsteroidView
{
public:
    AsteroidView(const std::shared_ptr<AsteroidModel> asteroidModel)
        : m_asteroidModel(asteroidModel)
    {
        GenerateAsteroidForm();
    }

    void Draw(ICanvas &canvas)
    {
        canvas.PushMatrix();
        canvas.Translate(m_asteroidModel->GetPosition());
        canvas.Rotate(m_asteroidModel->GetRotation());

        canvas.SetColor(Color(160, 140, 120, 255));
        canvas.FillPolygon(m_points);
        canvas.SetColor(Color(100, 85, 70, 200));
        canvas.DrawPolygon(m_points, 2.0f);

        // canvas.SetColor(0xFFFFFFFF);
        // canvas.DrawCircle({0, 0}, m_asteroidModel->GetCollision().GetRadius());

        canvas.PopMatrix();
    }

private:
    const std::shared_ptr<AsteroidModel> m_asteroidModel;
    std::vector<Point> m_points;
    const int SEGMENTS = 20;

    void GenerateAsteroidForm()
    {
        float radius = m_asteroidModel->GetRadius();
        m_points.clear();
        m_points.reserve(SEGMENTS);

        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_real_distribution<float> distPhase(0.0f, 2.0f * M_PI);

        float phase1 = distPhase(gen);
        float phase2 = distPhase(gen);

        for (int i = 0; i < SEGMENTS; i++)
        {
            float angle = (2.0f * M_PI * i) / SEGMENTS;

            float noise = 0.8f +
                          0.1 * sin(angle * 4.0f + phase1) +
                          0.1 * sin(angle * 7.0f + phase2);

            float r = radius * noise;
            m_points.emplace_back(r * cos(angle), r * sin(angle));
        }
    }
};