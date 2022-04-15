cmake_minimum_required(VERSION 3.18)
project(hd)
find_package(SDL2)
add_executable(hd hd.cpp)

target_link_libraries(hd SDL2)