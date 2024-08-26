cmake_minimum_required(VERSION 3.5)

project(evl LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(evl main.cpp
    abstractevent.hpp
    abstractsignalhandler.hpp
    app.hpp app.cpp
    eventloop.hpp eventloop.cpp
    functorsignalhandler.hpp
    methodsignalhandler.hpp
    signal.hpp
    signalhandlerevent.hpp
    tsqueue.hpp
    )
