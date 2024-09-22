cmake_minimum_required(VERSION 3.28)
project(WordlySimulator)

set(CMAKE_CXX_STANDARD 23)

add_executable(WordlySimulator main.cpp)
add_library(wordly src/Dictionary.cpp src/Game.cpp
        src/Parser.cpp)