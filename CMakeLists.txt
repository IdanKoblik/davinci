cmake_minimum_required(VERSION 4.0)
project(davinci C)

set(CMAKE_C_STANDARD 17)

add_executable(davinci main.c
        device.c
        device.h
        keyboard.c
        keyboard.h
)
