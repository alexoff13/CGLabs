cmake_minimum_required(VERSION 3.22)
project(Noise)

set(CMAKE_CXX_STANDARD 23)

add_executable(Noise main.cpp)
find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
target_link_libraries(Noise sfml-system sfml-graphics sfml-audio)
