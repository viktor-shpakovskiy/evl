#pragma once

#include "abstractevent.hpp"

namespace evl {

class EventLoopImpl;
struct EventLoopImplDeleter {
    void operator() (EventLoopImpl *p) const noexcept;
};

class EventLoop {
public:
    EventLoop() noexcept;

    int exec() const;

    void event(EventUp event) const noexcept;

    void quit() const;

private:
    mutable std::unique_ptr<EventLoopImpl, EventLoopImplDeleter> m_impl;
};

using EventLoopSp = std::shared_ptr<EventLoop>;
using EventLoopWp = std::weak_ptr<EventLoop>;

} // namespace evl

