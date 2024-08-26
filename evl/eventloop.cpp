#include "eventloop.hpp"
#include "tsqueue.hpp"

namespace evl {

class EventLoopImpl {
    friend class EventLoop;
    friend class QuitEvent;

    TSQueue<AbstractEventUp> events;

    mutable  bool run = true;

    int loop() {
        while (run) {
            auto event = events.pop();
            event->accept();
        }
        return 0;
    }
};

class QuitEvent final : public AbstractEvent {
public:
    QuitEvent(EventLoopImpl *el) noexcept
        : el(el) {}

    void accept() const override {
        el->run = false;
    }

private:
    EventLoopImpl *el;
};

EventLoop::EventLoop() noexcept
    : m_impl(new EventLoopImpl)
{
}

int EventLoop::exec() const
{
    return m_impl->loop();
}

void EventLoop::event(AbstractEventUp event) const noexcept
{
    m_impl->events.push(std::move(event));
}

void EventLoop::quit() const
{
    event(AbstractEventUp(new QuitEvent(m_impl.get())));
}

void EventLoopImplDeleter::operator()(EventLoopImpl *p) const noexcept {
    delete p;
}

} // namespace evl
