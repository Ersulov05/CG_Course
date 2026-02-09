#pragma once

#include <functional>
#include <memory>

class IObservable
{
public:
    using UpdateCallback = std::function<void()>;
    virtual ~IObservable() = default;
    virtual void Subscribe(const std::shared_ptr<void> &subscriber, UpdateCallback callback) = 0;
    virtual void Unsubscribe(const std::shared_ptr<void> &subscriber) = 0;
};