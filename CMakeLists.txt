cmake_minimum_required(VERSION 3.10)

# set the project name
project(Chip8)

# Use C++11
set(CMAKE_CXX_STANDARD 11)
# Require (at least) it
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Don't use e.g. GNU extension (like -std=gnu++11) for portability
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# add the executable
add_executable(Chip8 src/main.cpp)
target_link_libraries(Chip8 ${SDL2_LIBRARIES})
