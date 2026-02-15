#pragma once
#include "../model/Gallows.h"
#include "./Canvas/CCanvas.h"
#include "./ViewStrategy/IGallowsViewStrategy.h"
#include "./ViewStrategy/GallowsGraphicsViewStrategy.h"
#include "./ViewStrategy/GallowsTextViewStrategy.h"
#include "./Alphabet/AlphabetView.h"
#include "./Popup/Popup.h"
#include "./Button/Button.h"
#include <memory>
#include <iostream>

class GallowsView
{
public:
    const unsigned LETTER_SIZE = 24;
    const unsigned WIDTH = 1000;
    const unsigned HEIGHT = 600;

    GallowsView(Gallows &gallowsModel)
        : m_gallowsModel(gallowsModel),
          m_canvas(WIDTH, HEIGHT),
          m_alphabetView(std::make_unique<AlphabetView>(gallowsModel, m_canvas)),
          m_gameOverPopup(
              m_canvas,
              [this]()
              {
                  m_gallowsModel.Reset();
              }),
          m_gallowsViewStrategy(std::make_unique<GallowsTextViewStrategy>(gallowsModel)),
          m_button(
              m_canvas, {m_canvas.GetWidth() / 2 - 100, 500}, {200, 20}, 0xFFFFFFFF, 0xFFFFFFFF,
              [this]()
              {
                  ChangeViewStrategy();
              })
    {
        m_gallowsModel.OnChangeStateSubscribe(
            [this](GameState state)
            {
                if (state != GameState::Process)
                {
                    switch (state)
                    {
                    case GameState::Lose:
                        this->m_gameOverPopup.SetText(L"Проигрыш");
                        break;
                    case GameState::Win:
                        this->m_gameOverPopup.SetText(L"Победа");
                        break;

                    default:
                        break;
                    }

                    this->m_gameOverPopup.Open();
                }
            });
    }

    void Run()
    {
        m_canvas.RunWindow(
            [this](ICanvas &canvas)
            {
                this->m_alphabetView->Draw(canvas);
                ViewGallows();
                ViewHint();
                m_button.Draw(GetButtonText());
                if (this->m_gameOverPopup.IsOpened())
                {
                    this->m_gameOverPopup.Draw();
                }
            });
    }

private:
    CCanvas m_canvas;
    std::unique_ptr<IGallowsViewStrategy> m_gallowsViewStrategy;
    Gallows &m_gallowsModel;
    std::unique_ptr<AlphabetView> m_alphabetView;
    Popup m_gameOverPopup;
    Button m_button;

    void ViewGallows()
    {
        m_gallowsViewStrategy->Draw(m_canvas, m_gallowsModel.GetAttemptCount());
    }

    void ChangeViewStrategy()
    {
        switch (this->m_gallowsViewStrategy->GetType())
        {
        case GallowsViewType::Text:
            this->m_gallowsViewStrategy = std::make_unique<GallowsGraphicsViewStrategy>(m_gallowsModel);
            break;
        case GallowsViewType::Graphics:
        default:
            this->m_gallowsViewStrategy = std::make_unique<GallowsTextViewStrategy>(m_gallowsModel);
        }
    }

    void ViewHint()
    {
        auto hint = m_gallowsModel.GetWord().GetHint();
        auto textSize = m_canvas.GetTextSize(hint, LETTER_SIZE);
        m_canvas.DrawString(hint, {m_canvas.GetWidth() / 2 - textSize.width / 2, 0}, LETTER_SIZE);
    }

    std::wstring GetButtonText()
    {
        switch (m_gallowsViewStrategy->GetType())
        {
        case GallowsViewType::Graphics:
            return L"Текстовый режим";
        case GallowsViewType::Text:
        default:
            return L"Графический режим";
        }
    }
};