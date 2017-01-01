cmake_minimum_required(VERSION 3.6 FATAL_ERROR)

project(Verse CXX)

set(CMAKE_CXX_STANDARD 14)

include(${CMAKE_CURRENT_SOURCE_DIR}/Deliberation2/cmake/Platforms/Platform.cmake)

add_subdirectory(Deliberation2)
add_subdirectory(Source)
