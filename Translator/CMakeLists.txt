cmake_minimum_required(VERSION 3.11)
project(Translator)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -Wall -std=c++17")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -std=c++17")

set(CMAKE_CXX_COMPILER /usr/bin/g++-7)

include_directories(include)
add_subdirectory(src)