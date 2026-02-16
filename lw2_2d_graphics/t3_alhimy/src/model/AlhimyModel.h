#pragma once
#include <set>
#include "./Element.h"
#include "MixingElements.h"
#include <string>

struct CreatedElementsData
{
    std::vector<Element> createdElements;
    std::vector<Element> newlyCreatedElements;
};

class AlhimyModel
{
public:
    using OnEndGameCallback = std::function<void()>;

    AlhimyModel()
    {
        for (const auto &element : m_mixingElements.GetDefaultElements())
        {
            m_createdElements.insert(element);
        }
    }

    CreatedElementsData MixElements(const std::wstring &firstElementName, const std::wstring &secondElementName)
    {
        std::vector<Element> newlyCreatedElements;
        auto createdElements = m_mixingElements.MixElements(firstElementName, secondElementName);

        for (auto &element : createdElements)
        {
            if (m_createdElements.find(element) == m_createdElements.end())
            {
                m_createdElements.insert(element);
                newlyCreatedElements.push_back(element);

                if (m_createdElements.size() == m_mixingElements.GetElementsCount())
                {
                    NotifyEndGame();
                }
            }
        }

        return {createdElements, newlyCreatedElements};
    }

    std::vector<Element> GetCreatedElements() const
    {
        return std::vector<Element>(m_createdElements.begin(), m_createdElements.end());
    }

    unsigned OnEndGameSubscribe(OnEndGameCallback onEndGameCallback)
    {
        static unsigned nextId = 1;
        unsigned id = nextId++;
        m_callbacks[id] = onEndGameCallback;
        return id;
    }

    void OnEndGameUnsubscribe(unsigned subscriptionId)
    {
        m_callbacks.erase(subscriptionId);
    }

private:
    MixingElements m_mixingElements;
    std::set<Element> m_createdElements;
    std::unordered_map<unsigned, OnEndGameCallback> m_callbacks;

    void NotifyEndGame()
    {
        auto callbacksCopy = m_callbacks;
        for (const auto &[id, callback] : callbacksCopy)
        {
            if (callback)
            {
                callback();
            }
        }
    }
};