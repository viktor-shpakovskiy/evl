#include "app.hpp"

namespace evl {

EventLoop App::appLoop;

int App::exec()
{
    return appLoop.exec();
}

void App::event(AbstractEventUp event) noexcept
{
    appLoop.event(std::move(event));
}

void App::quit()
{
    appLoop.quit();
}

} // namespace evl
