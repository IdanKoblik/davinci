cmake_minimum_required(VERSION 4.0)
project(davinci)

set(CMAKE_CXX_STANDARD 20)

add_executable(davinci main.cpp
        keyboard.cpp
        keyboard.h
        device.cpp
        device.h
        config.cpp
        config.h
        trim.h
        trim.cpp
)
