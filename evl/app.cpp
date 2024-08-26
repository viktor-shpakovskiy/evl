#include "app.hpp"

namespace evl {

EventLoopSp App::appLoop = std::make_shared<EventLoop>();

int App::exec()
{
    return appLoop->exec();
}

void App::event(AbstractEventUp event) noexcept
{
    appLoop->event(std::move(event));
}

void App::quit() noexcept
{
    appLoop->quit();
}

EventLoopWp App::eventLoop() noexcept
{
    return appLoop;
}

} // namespace evl
