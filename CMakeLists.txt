cmake_minimum_required(VERSION 2.8.12)

project(topsort)
enable_testing()
SET(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

message(STATUS "Build type ${CMAKE_BUILD_TYPE}")

# set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
# -Weffc++
set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS} -Wall -Wextra -Wshadow -pedantic")
# create a compile database to be used by YCM
# set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

# set up testing
add_subdirectory(tests)
