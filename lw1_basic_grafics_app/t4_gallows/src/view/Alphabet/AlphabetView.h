#pragma once
#include "./LetterView.h"
#include <unordered_map>
#include <memory>
#include "../../model/Alphabet.h"
#include "../Common/Point.h"
#include "../Common/Size.h"
#include "../../model/Gallows.h"

class AlphabetView
{
public:
    AlphabetView(Gallows &gallowsModel, ICanvas &canvas) : m_alphabetModel(gallowsModel.GetAlphabet())
    {
        CreateLetters(gallowsModel, canvas);
        m_alphabetModel.OnChangeSubscribe(
            [this](const std::vector<LetterChange> &letterChanges)
            {
                for (auto &letterChange : letterChanges)
                {
                    this->m_letterViews[letterChange.letter]->SetState(letterChange.state);
                }
            });
    }

    ~AlphabetView()
    {
        if (m_subscriptionId != 0)
        {
            m_alphabetModel.OnChangeUnsubscribe(m_subscriptionId);
        }
    }

    void Draw(ICanvas &canvas)
    {
        for (auto &[letter, letterView] : m_letterViews)
        {
            letterView->Draw(canvas);
        }
    }

private:
    std::unordered_map<wchar_t, std::unique_ptr<LetterView>> m_letterViews;
    Alphabet &m_alphabetModel;
    unsigned m_subscriptionId;

    void CreateLetters(Gallows &gallowsModel, ICanvas &canvas)
    {
        auto alphabetLetters = m_alphabetModel.GetLetters();
        const unsigned size = 24;
        const unsigned interval = 10;
        Size alphabetSize = GetAlphabetSize(canvas, interval, size);
        float x = 500 - alphabetSize.width / 2;
        const int y = 450;

        auto &mouseController = canvas.GetMouseController();
        for (auto &[letter, state] : alphabetLetters)
        {
            auto boundSize = canvas.GetTextSize(letter, size);
            auto letterView = std::make_unique<LetterView>(
                letter, Point(x, y), size, boundSize, mouseController,
                [this, &gallowsModel](wchar_t clickedLetter)
                {
                    gallowsModel.GuessLetter(clickedLetter);
                });

            m_letterViews[letter] = std::move(letterView);

            x += boundSize.width + interval;
        }
    }

    Size GetAlphabetSize(ICanvas &canvas, unsigned interval, unsigned characterSize)
    {
        auto alphabetLetters = m_alphabetModel.GetLetters();
        Size size;
        for (auto &[letter, state] : alphabetLetters)
        {
            size += canvas.GetTextSize(letter, characterSize);
        }

        auto letterCount = alphabetLetters.size();
        size.width += interval * (letterCount - 1);

        return size;
    }
};