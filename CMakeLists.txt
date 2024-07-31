cmake_minimum_required(VERSION 3.10)
project(gravitySimV1.0)

find_package(SFML 2.5 REQUIRED graphics window system)

add_executable(GravitySimV1.0 main.cpp)
target_link_libraries(GravitySimV1.0 PRIVATE sfml-graphics sfml-window sfml-system)
