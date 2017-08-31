cmake_minimum_required(VERSION 3.6 FATAL_ERROR)

project(Verse CXX)

set(CMAKE_CXX_STANDARD 14)

include(${CMAKE_CURRENT_SOURCE_DIR}/Deliberation/CMake/Platforms/Platform.cmake)

add_subdirectory(Deliberation)
add_subdirectory(Source)
