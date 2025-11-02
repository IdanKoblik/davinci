cmake_minimum_required(VERSION 3.16)
project(davinci C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

file(GLOB SOURCES "*.c")
file(GLOB HEADERS "*.h")

add_executable(davinci ${SOURCES} ${HEADERS})


