#pragma once

#include "abstractsignalhandler.hpp"

namespace evl {

template <class Object, class Method, class... Args>
class MethodSignalHandler final : public AbstractSignalHandler<Args...> {
public:
    constexpr MethodSignalHandler(Object *object, Method method) noexcept
        : object(object), method(method) {}

    void call(Args... args) const override {
        (object->*method)(std::move(args)...);
    }

private:
    Object *object;
    Method method;
};

} // namespace evl
