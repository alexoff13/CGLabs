cmake_minimum_required(VERSION 3.22)
project(final)

set(CMAKE_CXX_STANDARD 23)

add_executable(final main.cpp)
find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
target_link_libraries(final sfml-system sfml-graphics sfml-audio)
