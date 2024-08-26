#pragma once

#include "eventloop.hpp"

namespace evl {

class App {
public:
    static int exec();

    static void event(AbstractEventUp event) noexcept;

    static void quit() noexcept;

    static EventLoopWp eventLoop() noexcept;

private:
    static EventLoopSp appLoop;
};

} // namespace evl

