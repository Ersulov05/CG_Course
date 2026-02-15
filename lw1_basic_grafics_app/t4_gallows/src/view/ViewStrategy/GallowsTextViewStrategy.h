#pragma once
#include "./IGallowsViewStrategy.h"
#include "../../model/Gallows.h"

const Size LETTER_PLACE_SIZE = Size(24, 3);
const unsigned PLACE_INTERVAL = 10;
const unsigned LETTER_SIZE = 24;

class GallowsTextViewStrategy : public IGallowsViewStrategy
{
public:
    GallowsTextViewStrategy(Gallows &gallowsModel)
        : m_gallowsModel(gallowsModel)
    {
    }

    void Draw(ICanvas &canvas, unsigned attemptCount) override
    {
        canvas.SetColor(0xFFFFFFFF);
        DrawAttempts(canvas, attemptCount);
        DrawWord(canvas);
    }

    GallowsViewType GetType()
    {
        return GallowsViewType::Text;
    }

private:
    Gallows &m_gallowsModel;

    void DrawAttempts(ICanvas &canvas, unsigned attemptCount)
    {
        std::wstring text = L"Осталось попыток: " + std::to_wstring(attemptCount);
        auto textSize = canvas.GetTextSize(text, 16);
        canvas.DrawString(text, {500 - textSize.width / 2, 250}, 16);
    }

    void DrawWord(ICanvas &canvas)
    {
        auto wordLetters = m_gallowsModel.GetWord().GetDisplayWord();
        int x = canvas.GetWidth() / 2 - GetPlaceWordWidth(wordLetters.size()) / 2;
        for (auto &wordLetter : wordLetters)
        {
            auto boundSize = canvas.GetTextSize(wordLetter, LETTER_SIZE);
            canvas.DrawChar(wordLetter, {x + LETTER_PLACE_SIZE.width / 2 - boundSize.width / 2, 200}, LETTER_SIZE);
            canvas.FillRect({x, 230}, LETTER_PLACE_SIZE);
            x += LETTER_PLACE_SIZE.width + PLACE_INTERVAL;
        }
    }

    unsigned GetPlaceWordWidth(unsigned lettersCount)
    {
        return lettersCount * (LETTER_PLACE_SIZE.width + PLACE_INTERVAL) - PLACE_INTERVAL;
    }
};