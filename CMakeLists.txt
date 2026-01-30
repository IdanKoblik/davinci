cmake_minimum_required(VERSION 3.20)
project(davinci LANGUAGES C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    kutiles
    GIT_REPOSITORY https://github.com/IdanKoblik/kutiles.git
    GIT_TAG 2026-b1
)
FetchContent_MakeAvailable(kutiles)

file(GLOB DAVINCI_SRC src/*.c)

add_library(davinci_lib ${DAVINCI_SRC})
target_include_directories(davinci_lib
    PUBLIC include
)
target_link_libraries(davinci_lib
    PUBLIC kutiles::kutiles
)
target_compile_options(davinci_lib PRIVATE -Wall -Wextra -Wpedantic)

add_library(davinci::davinci_lib ALIAS davinci_lib)

add_executable(davinci src/main.c)
target_link_libraries(davinci
    PRIVATE davinci_lib
)

enable_testing()
add_subdirectory(tests)
