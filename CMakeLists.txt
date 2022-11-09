cmake_minimum_required(VERSION 3.23)

project(
        secs
        LANGUAGES CXX
        DESCRIPTION "A simplistic implementation of an ECS, written in C++"
)

set(CMAKE_CXX_STANDARD 14)

option(BUILD_TESTING "" OFF)
include(CTest)

if(BUILD_TESTING)
    enable_testing()
endif()

add_subdirectory(src)

if(BUILD_TESTING)
    add_subdirectory(test)
endif()