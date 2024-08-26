#pragma once

#include "app.hpp"
#include "abstractsignalhandler.hpp"
#include "functorsignalhandler.hpp"
#include "methodsignalhandler.hpp"
#include "signalhandlerevent.hpp"

#include <memory>
#include <list>

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

template <class... Args>
class Signal final {
public:
    Signal() noexcept = default;
    virtual ~Signal() = default;

    using SignalHandler = AbstractSignalHandler<Args...>;

    template <class Functor>
    ConnectionSp<Args...>
    connect(Functor &&functor, ConnectionType type = DirectConnection) const noexcept {
        auto handler = new FunctorSignalHanlder<Functor, Args...>(functor);
        return connectHandler(handler, type);
    }

    template <class Object, class Method>
    ConnectionSp<Args...>
    connect(Object *object, Method method, ConnectionType type = DirectConnection) const noexcept {
        auto handler = new MethodSignalHandler<Object, Method, Args...> (object, method);
        return connectHandler(handler, type);
    }

    void emit(Args... args) const {
        for (auto ci = w_connections.begin();
             ci != w_connections.end();)
        {
            auto connection = ci->lock();
            if (connection) {
                if (connection->enabled) {
                    if (connection->type == DirectConnection)
                        connection->handler->call(std::move(args)...);
                    else
                        App::event(std::move(connection->makeEvent(std::move(args)...)));
                }
                ++ci;
            }
            else
                ci = w_connections.erase(ci);
        }
    }

    void operator() (Args... args) const {
        emit(std::move(args)...);
    }

private:
    mutable std::list<ConnectionWp<Args...>> w_connections;

    inline ConnectionSp<Args...>
    connectHandler(AbstractSignalHandler<Args...> *handler,
                   ConnectionType type) const noexcept {
        auto connection = ConnectionSp<Args...>(new Connection<Args...>(handler, type));
        w_connections.emplace_back(std::move(connection));
        return connection;
    }
};

} // namespace evl
