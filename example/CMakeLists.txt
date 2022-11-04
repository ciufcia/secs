cmake_minimum_required(VERSION 3.23)

project(secs-example)

set(CMAKE_CXX_STANDARD 14)

include(FetchContent)

set(BUILD_SHARED_LIBS OFF)

FetchContent_Declare(
        sfml
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG master
)
FetchContent_MakeAvailable(sfml)

FetchContent_Declare(
        secs
        GIT_REPOSITORY https://github.com/ciufcia/secs.git
        GIT_TAG main
)
FetchContent_MakeAvailable(secs)

add_executable(example example.cpp)

target_link_libraries(example secs sfml-graphics)