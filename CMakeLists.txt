cmake_minimum_required(VERSION 3.10)
project(hello)

find_package(SFML 2.5 REQUIRED graphics window system)

add_executable(hello main.cpp)
target_link_libraries(hello PRIVATE sfml-graphics sfml-window sfml-system)
