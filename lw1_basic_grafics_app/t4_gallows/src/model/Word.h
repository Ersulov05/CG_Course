#pragma once
#include <string>
#include <unordered_set>
#include <vector>
#include <functional>
#include <cwctype>
#include <iostream>

class Word
{
public:
    Word() = default;
    Word(const std::wstring &word, const std::wstring &hint)
    {
        initLettersByWord(word);
        m_word = word;
        m_hint = hint;
    }

    bool GuessLetter(wchar_t letter)
    {
        letter = std::towupper(letter);

        auto it = m_lettersMap.find(letter);
        if (it != m_lettersMap.end())
        {
            it->second = true;
            return true;
        }

        return false;
    }

    bool IsCompletelyGuessed() const
    {
        for (const auto &pair : m_lettersMap)
        {
            if (!pair.second)
            {
                return false;
            }
        }
        return true;
    }

    std::vector<wchar_t> GetDisplayWord() const
    {
        std::vector<wchar_t> display;
        for (wchar_t c : m_word)
        {
            wchar_t upperC = std::towupper(c);
            auto it = m_lettersMap.find(upperC);

            if (it != m_lettersMap.end() && it->second)
            {
                display.push_back(c);
            }
            else
            {
                display.push_back(' ');
            }
        }
        return display;
    }

    std::wstring GetHint() const { return m_hint; }

private:
    void initLettersByWord(const std::wstring &word)
    {
        m_lettersMap.clear();
        for (wchar_t letter : word)
        {
            wchar_t upperLetter = std::towupper(letter);
            m_lettersMap.try_emplace(upperLetter, false);
        }
    }

    std::wstring m_word;
    std::wstring m_hint;
    std::unordered_map<wchar_t, bool> m_lettersMap;
};