#pragma once
#include "./Word.h"
#include "./WordGenerator.h"
#include "./Alphabet.h"

const unsigned MAX_ATTEMPT = 5;

class Gallows
{
public:
    Gallows()
    {
        InitGallows();
    }

    void Reset()
    {
        InitGallows();
    }

    unsigned GetAttemptCount()
    {
        return m_attemptCount;
    }

    void GuessLetter(wchar_t letter)
    {
        auto isCorrect = m_word.GuessLetter(letter);
        LetterState state = m_word.GuessLetter(letter)
                                ? LetterState::Correct
                                : LetterState::Wrong;

        m_alphabet.SetLetterState(letter, state);
    }

    Alphabet GetAlphabet()
    {
        return m_alphabet;
    }

    Word GetWord()
    {
        return m_word;
    }

private:
    void InitGallows()
    {
        m_attemptCount = MAX_ATTEMPT;
        m_word = WordGenerator::GenerateWord();
        m_alphabet.Reset();
    }

    unsigned m_attemptCount = MAX_ATTEMPT;
    Word m_word;
    Alphabet m_alphabet;
};