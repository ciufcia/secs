cmake_minimum_required(VERSION 3.23)

project(
    secs
    LANGUAGES CXX
    DESCRIPTION "A simplistic implementation of an ECS written in C++"
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

option(BUILD_TESTING "Build tests" OFF)
option(BUILD_EXAMPLES "Build examples" OFF)

add_subdirectory(src)

if (BUILD_TESTING)
    include(CTest)
    enable_testing()
    add_subdirectory(test)
endif()

if (BUILD_EXAMPLES)
    add_subdirectory(example)
endif()