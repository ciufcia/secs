cmake_minimum_required(VERSION 3.23)

project(
        secs
        LANGUAGES CXX
        DESCRIPTION "A simplistic implementation of an ECS, written in C++"
)

set(CMAKE_CXX_STANDARD 14)

include(CTest)

enable_testing()

add_subdirectory(src)
add_subdirectory(test)