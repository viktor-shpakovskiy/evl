#include <iostream>

#include "signal.hpp"

using namespace evl;

int main()
{
    Signal<> signal;

    auto connection = signal.connect([] {
        std::cout << "Hello\n";
    }, QueuedConnection);

    // signal();
    signal.emit();

    App::quit();

    return App::exec();
}
