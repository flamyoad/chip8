cmake_minimum_required(VERSION 3.10)

# set the project name
project(Chip8)

# Use C++11
set(CMAKE_CXX_STANDARD 11)
# Require (at least) it
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Don't use e.g. GNU extension (like -std=gnu++11) for portability
set(CMAKE_CXX_EXTENSIONS OFF)

if(WIN32)
    set(SDL2_DIR ${CMAKE_CURRENT_LIST_DIR}/deps/SDL2-2.30.8/cmake/)
endif()

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# add the executable
add_executable(Chip8 src/main.cpp)
target_link_libraries(Chip8 ${SDL2_LIBRARIES})

# Copy SDL2 DLL to build directory (Windows only)
if(WIN32)
    add_custom_command(TARGET Chip8 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_LIST_DIR}/deps/SDL2-2.30.8/x86_64-w64-mingw32/bin/SDL2.dll"
        $<TARGET_FILE_DIR:Chip8>
    )
endif()
