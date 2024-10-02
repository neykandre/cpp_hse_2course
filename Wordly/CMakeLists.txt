cmake_minimum_required(VERSION 3.28)
project(WordlySimulator)

set(CMAKE_CXX_STANDARD 23)

add_library(Wordly STATIC src/Parser.cpp src/Dictionary.cpp src/Game.cpp)
add_executable(WordlySimulator main.cpp)
target_link_libraries(WordlySimulator Wordly)
