cmake_minimum_required(VERSION 3.6 FATAL_ERROR)

project(Verse CXX)

# CMake configuration
set(CMAKE_CXX_STANDARD             14)
set(CMAKE_MODULE_PATH 			   ${CMAKE_CURRENT_SOURCE_DIR}/CMake ${CMAKE_MODULE_PATH})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

include(${CMAKE_CURRENT_SOURCE_DIR}/Deliberation/CMake/Platforms/Platform.cmake)

add_subdirectory(Deliberation)
add_subdirectory(Source)
