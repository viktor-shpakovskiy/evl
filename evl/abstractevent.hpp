#pragma once

#include <memory>

namespace evl {

class AbstractEvent {
public:
    virtual ~AbstractEvent() = default;
    virtual void accept() const = 0;

protected:
    AbstractEvent() noexcept = default;
};

using EventUp = std::unique_ptr<AbstractEvent>;

} // namespace evl
