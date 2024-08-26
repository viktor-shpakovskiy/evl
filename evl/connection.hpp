#pragma once

#include "abstractsignalhandler.hpp"
#include "signalhandlerevent.hpp"

namespace evl {

enum ConnectionType {
    DirectConnection,
    QueuedConnection,
};

template <class... Args>
class Signal;

template <class... Args>
class Connection final {
    friend class Signal<Args...>;

public:
    void enable() const noexcept {
        enabled = true;
    }

    void disable() const noexcept {
        enabled = false;
    }

    void toggle() const noexcept {
        enabled = !enabled;
    }

private:
    using SignalHandler = AbstractSignalHandler<Args...>;
    using SignalHandlerSp = AbstractSignalHandlerSp<Args...>;

    Connection(SignalHandler *handler, ConnectionType type) noexcept
        : type(type), enabled(true), handler(handler) {}

    const ConnectionType type;
    mutable bool enabled;
    SignalHandlerSp handler;

    EventUp makeEvent(Args... args) const noexcept {
        return EventUp(new SignalHandlerEvent<Args...>(handler, std::move(args)...));
    }
};

template <class... Args>
using ConnectionSp = std::shared_ptr<Connection<Args...>>;

template <class... Args>
using ConnectionWp = std::weak_ptr<Connection<Args...>>;

} // namespace evl
