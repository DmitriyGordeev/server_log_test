cmake_minimum_required(VERSION 3.5.1)
project(GameLogParser)

set(CMAKE_CXX_STANDARD 11)

set(FILEIO_SOURCE fileio/fileio.cpp fileio/fileio.h)

# main project:
set(SOURCE_FILES main.cpp user-action.h Date.h Agr.cpp Date.cpp)
add_executable(GameLogParser ${SOURCE_FILES} ${FILEIO_SOURCE})

# dataset creator
add_executable(create_dataset ${FILEIO_SOURCE} create_dataset.cpp)