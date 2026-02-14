#pragma once
#include <string>
#include <unordered_set>
#include <vector>
#include <functional>

class Word
{
public:
    Word() = default;
    Word(const std::wstring &word, const std::wstring &hint)
    {
        m_word = word;
        m_hint = hint;
    }

    bool GuessLetter(wchar_t letter)
    {
        letter = std::toupper(letter);

        auto it = m_lettersMap.find(letter);
        if (it != m_lettersMap.end() && !it->second)
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
            wchar_t upperC = std::toupper(c);
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
    // void initLettersByWord(const std::string &word)
    // {
    //     m_lettersMap.clear();

    //     for (char letter : word)
    //     {
    //         m_lettersMap(Letter(letter));
    //     }
    // }

    void initLettersByWord(const std::wstring &word)
    {
        m_lettersMap.clear();
        for (char letter : word)
        {
            if (std::isalpha(letter))
            {
                char upperLetter = std::toupper(letter);
                m_lettersMap.try_emplace(upperLetter, false);
            }
        }
    }

    std::wstring m_word;
    std::wstring m_hint;
    std::unordered_map<char, bool> m_lettersMap;
};