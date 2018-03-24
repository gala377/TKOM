cmake_minimum_required(VERSION 3.9)
project(Translator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -Wall -std=c++17")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -04 -m66 -std=c++17")

include_directories(include)
add_subdirectory(src)


set_target_properties( Translator
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY_DEBUG "build/debug/lib"
        LIBRARY_OUTPUT_DIRECTORY_DEBUG "build/debug/lib"
        RUNTIME_OUTPUT_DIRECTORY_DEBUG "build/debug/bin"
        ARCHIVE_OUTPUT_DIRECTORY_RELEASE "build/release/lib"
        LIBRARY_OUTPUT_DIRECTORY_RELEASE "build/release/lib"
        RUNTIME_OUTPUT_DIRECTORY_RELEASE "build/release/bin"
        )