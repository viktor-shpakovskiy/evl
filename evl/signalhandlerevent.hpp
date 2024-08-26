#pragma once

#include "abstractevent.hpp"
#include "abstractsignalhandler.hpp"

#include <tuple>
#include <utility>

namespace evl {

template <class... Args>
class Connection;

template <class... Args>
class SignalHandlerEvent final : AbstractEvent {
    friend class Connection<Args...>;

public:
    void accept() const override {
        call(std::index_sequence_for<Args...>{});
    }

private:
    using SignalHandlerWp = AbstractSignalHandlerWp<Args...>;

    SignalHandlerEvent(SignalHandlerWp handler, Args... args) noexcept
        : w_handler(handler), args(std::move(args)...) {}

    SignalHandlerWp w_handler;
    std::tuple<Args...> args;

    template <std::size_t... I>
    void call(std::index_sequence<I...>) const {
        auto handler = w_handler.lock();
        if (handler)
            handler->call(std::move(std::get<I>(args))...);
    }
};

} // namespace evl
