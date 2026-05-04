cmake_minimum_required(VERSION 3.16)
project(NickTetris)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(NickTetris src/main.cpp)

find_package(raylib REQUIRED)

target_link_libraries(NickTetris PRIVATE raylib)