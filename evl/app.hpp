#pragma once

#include "eventloop.hpp"

namespace evl {

class App {
public:
    static int exec();

    static void event(EventUp event) noexcept;

    static void quit();

private:
    static EventLoop appLoop;
};

} // namespace evl

