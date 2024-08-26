#pragma once

#include "abstractsignalhandler.hpp"
#include "functorsignalhandler.hpp"
#include "methodsignalhandler.hpp"
#include "connection.hpp"

#include <list>

namespace evl {

template <class... Args>
class Signal final {
public:
    Signal() noexcept = default;
    virtual ~Signal() = default;

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
                connection->handle();
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
    using SignalHandler = AbstractSignalHandler<Args...>;

    mutable std::list<ConnectionWp<Args...>> w_connections;

    inline ConnectionSp<Args...>
    connectHandler(SignalHandler *handler, ConnectionType type) const noexcept {
        auto connection = ConnectionSp<Args...>(new Connection<Args...>(handler, type));
        w_connections.emplace_back(std::move(connection));
        return connection;
    }
};

} // namespace evl
