#pragma once
#include "./Word.h"
#include "./WordGenerator.h"
#include "./Alphabet.h"

const unsigned MAX_ATTEMPT = 6;

enum class GameState
{
    Process,
    Win,
    Lose
};

class Gallows
{
public:
    using OnChangeStateCallback = std::function<void(GameState)>;

    Gallows()
    {
        InitGallows();
    }

    void Reset()
    {
        InitGallows();
        m_state = GameState::Process;
        NotifyCallbacks(m_state);
    }

    unsigned GetAttemptCount()
    {
        return m_attemptCount;
    }

    void GuessLetter(wchar_t letter)
    {
        if (m_state != GameState::Process)
        {
            return;
        }
        auto letterState = m_alphabet.GetLetterState(letter);
        if (letterState != LetterState::NotUsed)
        {
            return;
        }

        bool isCorrect = m_word.GuessLetter(letter);
        if (!isCorrect)
        {
            --m_attemptCount;
        }
        letterState = isCorrect
                          ? LetterState::Correct
                          : LetterState::Wrong;

        m_alphabet.SetLetterState(letter, letterState);
        CheckGameState();
    }

    Alphabet &GetAlphabet()
    {
        return m_alphabet;
    }

    Word GetWord()
    {
        return m_word;
    }

    unsigned OnChangeStateSubscribe(OnChangeStateCallback onChangeStateCallback)
    {
        static unsigned nextId = 1;
        unsigned id = nextId++;
        m_callbacks[id] = onChangeStateCallback;
        return id;
    }

    void OnChangeStateUnsubscribe(unsigned subscriptionId)
    {
        m_callbacks.erase(subscriptionId);
    }

private:
    unsigned m_attemptCount;
    Word m_word;
    Alphabet m_alphabet;
    GameState m_state = GameState::Process;
    std::unordered_map<unsigned, OnChangeStateCallback> m_callbacks;

    void CheckGameState()
    {
        if (m_attemptCount == 0)
        {
            m_state = GameState::Lose;
            NotifyCallbacks(m_state);
        }
        if (m_word.IsCompletelyGuessed())
        {
            m_state = GameState::Win;
            NotifyCallbacks(m_state);
        }
    }

    void NotifyCallbacks(GameState state)
    {
        auto callbacksCopy = m_callbacks;
        for (const auto &[id, callback] : callbacksCopy)
        {
            if (callback)
            {
                callback(state);
            }
        }
    }

    void InitGallows()
    {
        m_attemptCount = MAX_ATTEMPT;
        m_word = WordGenerator::GenerateWord();
        m_alphabet.Reset();
    }
};