#pragma once

#include "abstractsignalhandler.hpp"

namespace evl {

template <class Functor, class... Args>
struct IsFunctorArgsCompatible {
private:
    template<class CheckedFunctor, class... CheckedArgs>
    static constexpr std::true_type exists(
            decltype(std::declval<CheckedFunctor>()
                     (std::declval<CheckedArgs>()...))* = nullptr);

    template<class CheckedFunctor, class... CheckedArgs>
    static constexpr std::false_type exists(...);

public:
    static constexpr bool value =
            decltype(exists<Functor, Args...>(nullptr))::value;
};

template <class Functor, class... Args>
class FunctorSignalHanlder final : public AbstractSignalHandler<Args...> {
public:
    constexpr FunctorSignalHanlder(Functor &functor) noexcept
        : functor(functor) {}

    void call(Args... args) const override {
        static_assert (
            IsFunctorArgsCompatible<Functor, Args...>::value,
            "evl::Signal and Functor arguments are not compatible"
        );
        functor(std::move(args)...);
    }

private:
    Functor &functor;
};

} // namespace evl
