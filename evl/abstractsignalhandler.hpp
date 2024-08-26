#pragma once

#include <algorithm>
#include <memory>

namespace evl {

template <class... Args>
class AbstractSignalHandler {
public:
    virtual ~AbstractSignalHandler() = default;
    virtual void call(Args... args) const = 0;

protected:
    constexpr AbstractSignalHandler() noexcept = default;
};

template <class... Args>
using AbstractSignalHandlerSp = std::shared_ptr<AbstractSignalHandler<Args...>>;

template <class... Args>
using AbstractSignalHandlerWp = std::weak_ptr<AbstractSignalHandler<Args...>>;

} // namespace evl
