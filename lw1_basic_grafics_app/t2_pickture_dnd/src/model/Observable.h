#pragma once

#include "IObservable.h"
#include <map>

class Observable : public IObservable
{
protected:
    void NotifySubscribers()
    {
        auto subscribers = m_subscribers;
        for (const auto &[subscriber, callback] : subscribers)
        {
            if (callback)
                callback();
        }
    }

private:
    std::map<std::shared_ptr<void>, UpdateCallback> m_subscribers;

    void Subscribe(const std::shared_ptr<void> &subscriber, UpdateCallback callback) override
    {
        m_subscribers[subscriber] = callback;
    }

    void Unsubscribe(const std::shared_ptr<void> &subscriber) override
    {
        m_subscribers.erase(subscriber);
    }
};