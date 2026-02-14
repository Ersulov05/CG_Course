#pragma once
#include "../model/Gallows.h"
#include "./Canvas/CCanvas.h"
#include "./ViewStrategy/IGallowsViewStrategy.h"
#include "./ViewStrategy/GallowsPerformanceViewStrategy.h"
#include <memory>
#include <iostream>

class GallowsView
{
public:
    GallowsView(Gallows &gallowsModel)
        : m_gallowsModel(gallowsModel),
          m_canvas(CCanvas(800, 600))
    {
        m_gallowsViewStrategy = std::make_unique<GallowsPerformanceViewStrategy>();
        auto &mouseController = m_canvas.GetMouseController();
        mouseController.OnClickSubscribe(
            [this](const Point &mousePosition)
            {
                std::cout << "click" << std::endl;
                // if (mousePosition.x >= x &&
                //     mousePosition.x <= x + size &&
                //     mousePosition.y >= y &&
                //     mousePosition.y <= y + size)
                // {
                //     m_gallowsModel.GuessLetter(letter);
                // }
            });
    }

    void Run()
    {
        m_canvas.RunWindow(
            [this](ICanvas &canvas)
            {
                this->ViewAlphabet();
                this->ViewGallows();
                this->ViewWord();
            });
    }

private:
    CCanvas m_canvas;
    std::unique_ptr<IGallowsViewStrategy> m_gallowsViewStrategy;
    Gallows &m_gallowsModel;

    void ViewAlphabet()
    {
        auto alphabetLetters = m_gallowsModel.GetAlphabet().GetLetters();
        int x = 0;
        const int size = 24;
        const int y = 300;
        auto &mouseController = m_canvas.GetMouseController();
        for (auto &[letter, state] : alphabetLetters)
        {
            switch (state)
            {
            case LetterState::Correct:
                m_canvas.SetColor(0x00FF00FF);
                break;
            case LetterState::Wrong:
                m_canvas.SetColor(0xFF0000FF);
                break;
            default:
                mouseController.OnClickSubscribe(
                    [this, letter, x, y](const Point &mousePosition)
                    {
                        std::cout << "click" << std::endl;
                        if (mousePosition.x >= x &&
                            mousePosition.x <= x + size &&
                            mousePosition.y >= y &&
                            mousePosition.y <= y + size)
                        {
                            m_gallowsModel.GuessLetter(letter);
                        }
                    });

                m_canvas.SetColor(0x888888FF);
                break;
            }

            m_canvas.DrawChar(letter, {x, y}, size);
            x += size;
        }
    }

    void ViewGallows()
    {
        m_gallowsViewStrategy->Draw(m_canvas, m_gallowsModel.GetAttemptCount());
    }

    void ViewWord()
    {
        auto wordLetters = m_gallowsModel.GetWord().GetDisplayWord();
        for (auto &wordLetter : wordLetters)
        {
            // draw
        }
    }
};