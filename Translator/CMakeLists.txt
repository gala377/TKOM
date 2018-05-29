cmake_minimum_required(VERSION 3.9)
project(Translator)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -Wall -std=c++17")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -std=c++17")

set(CMAKE_CXX_COMPILER /usr/bin/g++-7)

include_directories(include)
add_subdirectory(src)


set_target_properties( Translator
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY_DEBUG "debug/lib"
        LIBRARY_OUTPUT_DIRECTORY_DEBUG "debug/lib"
        RUNTIME_OUTPUT_DIRECTORY_DEBUG "debug/bin"
        ARCHIVE_OUTPUT_DIRECTORY_RELEASE "release/lib"
        LIBRARY_OUTPUT_DIRECTORY_RELEASE "release/lib"
        RUNTIME_OUTPUT_DIRECTORY_RELEASE "release/bin"
        )