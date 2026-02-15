#pragma once
#include "./IGallowsViewStrategy.h"
#include "../../model/Gallows.h"

class GallowsGraphicsViewStrategy : public IGallowsViewStrategy
{
public:
    GallowsGraphicsViewStrategy(Gallows &gallowsModel)
        : m_position(Point(100, 50)),
          m_gallowsModel(gallowsModel)
    {
    }

    void Draw(ICanvas &canvas, unsigned attemptCount) override
    {
        DrawGallowsBase(canvas);
        DrawHangman(canvas, attemptCount);
        canvas.SetColor(0xFFFFFFFF);
        DrawWord(canvas);
    }

    GallowsViewType GetType()
    {
        return GallowsViewType::Graphics;
    }

private:
    Point m_position;
    Gallows &m_gallowsModel;

    void DrawWord(ICanvas &canvas)
    {
        auto wordLetters = m_gallowsModel.GetWord().GetDisplayWord();
        int x = 400;
        unsigned size = 24;
        Size placeSize(24, 3);
        for (auto &wordLetter : wordLetters)
        {
            auto boundSize = canvas.GetTextSize(wordLetter, size);
            canvas.DrawChar(wordLetter, {x + placeSize.width / 2 - boundSize.width / 2, 200}, size);
            canvas.FillRect({x, 230}, placeSize);
            x += placeSize.width + 10;
        }
    }

    void DrawGallowsBase(ICanvas &canvas)
    {
        canvas.SetColor(0x8B4513FF); // Коричневый

        // Основание (горизонтальная балка внизу)
        std::vector<Point> base = {
            {m_position.x - 50, m_position.y + 300},
            {m_position.x + 150, m_position.y + 300},
            {m_position.x + 150, m_position.y + 320},
            {m_position.x - 50, m_position.y + 320}};
        canvas.FillPolygon(base);

        // Вертикальный столб
        std::vector<Point> pillar = {
            {m_position.x, m_position.y},
            {m_position.x + 20, m_position.y},
            {m_position.x + 20, m_position.y + 300},
            {m_position.x, m_position.y + 300}};
        canvas.FillPolygon(pillar);

        // Верхняя перекладина
        std::vector<Point> topBar = {
            {m_position.x, m_position.y},
            {m_position.x + 150, m_position.y},
            {m_position.x + 150, m_position.y + 20},
            {m_position.x, m_position.y + 20}};
        canvas.FillPolygon(topBar);

        // Веревка
        canvas.SetColor(0xAAAAAAFF); // Серый
        std::vector<Point> rope = {
            {m_position.x + 136, m_position.y + 20},
            {m_position.x + 144, m_position.y + 20},
            {m_position.x + 144, m_position.y + 60},
            {m_position.x + 136, m_position.y + 60}};
        canvas.FillPolygon(rope);
    }

    void DrawHangman(ICanvas &canvas, unsigned attemptCount)
    {
        // Цвет для человека
        canvas.SetColor(0xFFFFFFFF); // Белый
        if (attemptCount < 6)
        {
            canvas.FillEllipse(m_position.x + 140, m_position.y + 80, 20, 20);
        }

        if (attemptCount < 5)
        {
            std::vector<Point> body = {
                {m_position.x + 134, m_position.y + 100},
                {m_position.x + 146, m_position.y + 100},
                {m_position.x + 146, m_position.y + 180},
                {m_position.x + 134, m_position.y + 180}};
            canvas.FillPolygon(body);
        }

        if (attemptCount < 4)
        {
            std::vector<Point> leftArm = {
                {m_position.x + 135, m_position.y + 100},
                {m_position.x + 110, m_position.y + 150},
                {m_position.x + 110, m_position.y + 162},
                {m_position.x + 135, m_position.y + 112}};
            canvas.FillPolygon(leftArm);
        }

        if (attemptCount < 3)
        {
            std::vector<Point> rightArm = {
                {m_position.x + 145, m_position.y + 100},
                {m_position.x + 170, m_position.y + 150},
                {m_position.x + 170, m_position.y + 162},
                {m_position.x + 145, m_position.y + 112}};
            canvas.FillPolygon(rightArm);
        }

        if (attemptCount < 2)
        {
            std::vector<Point> leftLeg = {
                {m_position.x + 135, m_position.y + 168},
                {m_position.x + 110, m_position.y + 218},
                {m_position.x + 110, m_position.y + 230},
                {m_position.x + 135, m_position.y + 180}};
            canvas.FillPolygon(leftLeg);
        }

        // Правая нога
        if (attemptCount < 1)
        {
            std::vector<Point> rightLeg = {
                {m_position.x + 145, m_position.y + 168},
                {m_position.x + 170, m_position.y + 218},
                {m_position.x + 170, m_position.y + 230},
                {m_position.x + 145, m_position.y + 180}};
            canvas.FillPolygon(rightLeg);
        }
    }
};