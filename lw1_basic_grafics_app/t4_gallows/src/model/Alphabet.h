#pragma once
#include <string>
#include <unordered_set>
#include <map>
#include <cwctype>

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

struct LetterChange
{
    wchar_t letter;
    LetterState state;
};

class Alphabet
{
public:
    using OnChangeCallback = std::function<void(const std::vector<LetterChange> &)>;

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
        letter = std::towupper(letter);

        auto it = m_lettersMap.find(letter);
        if (it != m_lettersMap.end())
        {
            it->second = state;
            NotifyCallbacks({LetterChange(letter, state)});
        }
    }

    LetterState GetLetterState(wchar_t letter)
    {
        letter = std::towupper(letter);

        auto it = m_lettersMap.find(letter);
        if (it != m_lettersMap.end())
        {
            return it->second;
        }

        return LetterState::NotUsed;
    }

    std::map<wchar_t, LetterState, RussianAlphabetComparator> GetLetters()
    {
        return m_lettersMap;
    }

    unsigned OnChangeSubscribe(OnChangeCallback onChangeCallback)
    {
        static unsigned nextId = 1;
        unsigned id = nextId++;
        m_callbacks[id] = onChangeCallback;
        return id;
    }

    void OnChangeUnsubscribe(unsigned subscriptionId)
    {
        m_callbacks.erase(subscriptionId);
    }

private:
    void InitAlphabet()
    {
        std::vector<LetterChange> changes;
        m_lettersMap.clear();
        const wchar_t *russianAlphabet = L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";

        for (int i = 0; russianAlphabet[i] != L'\0'; ++i)
        {
            m_lettersMap[russianAlphabet[i]] = LetterState::NotUsed;
            changes.push_back(LetterChange(russianAlphabet[i], LetterState::NotUsed));
        }
        NotifyCallbacks(changes);
    }

    void NotifyCallbacks(const std::vector<LetterChange> &changes)
    {
        auto callbacksCopy = m_callbacks;
        for (const auto &[id, callback] : callbacksCopy)
        {
            if (callback)
            {
                callback(changes);
            }
        }
    }

    std::map<wchar_t, LetterState, RussianAlphabetComparator> m_lettersMap;
    std::unordered_map<unsigned, OnChangeCallback> m_callbacks;
};