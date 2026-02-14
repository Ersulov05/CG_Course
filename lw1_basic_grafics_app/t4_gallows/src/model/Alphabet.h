#pragma once
#include <string>
#include <unordered_set>
#include <map>

enum class LetterState
{
    NotUsed, // Серая - еще не использовалась
    Correct, // Зеленая - угадана правильно
    Wrong    // Красная - использована, но не угадана
};

struct RussianAlphabetComparator
{
    bool operator()(wchar_t a, wchar_t b) const
    {
        return GetRussianAlphabetOrder(a) < GetRussianAlphabetOrder(b);
    }

private:
    int GetRussianAlphabetOrder(wchar_t c) const
    {
        static const std::wstring order = L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
        auto pos = order.find(c);
        return (pos != std::wstring::npos) ? static_cast<int>(pos) : 999;
    }
};

class Alphabet
{
public:
    Alphabet()
    {
        InitAlphabet();
    }

    void Reset()
    {
        InitAlphabet();
    }

    void SetLetterState(wchar_t letter, LetterState state)
    {
        letter = std::toupper(letter);

        auto it = m_alphabetMap.find(letter);
        if (it != m_alphabetMap.end())
        {
            it->second = state;
        }
    }

    std::map<wchar_t, LetterState, RussianAlphabetComparator> GetLetters()
    {
        return m_alphabetMap;
    }

private:
    void InitAlphabet()
    {
        m_alphabetMap.clear();
        const wchar_t *russianAlphabet = L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";

        for (int i = 0; russianAlphabet[i] != L'\0'; ++i)
        {
            m_alphabetMap[russianAlphabet[i]] = LetterState::NotUsed;
        }
    }

    std::map<wchar_t, LetterState, RussianAlphabetComparator> m_alphabetMap;
};