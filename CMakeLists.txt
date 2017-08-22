cmake_minimum_required(VERSION 3.5.1)
project(GameLogParser)

set(CMAKE_CXX_STANDARD 11)

# main project:
set(SOURCE_FILES main.cpp fileio/fileio.cpp fileio/fileio.h fileio/test_fileio.cpp)
add_executable(GameLogParser ${SOURCE_FILES})

# tests:
set()