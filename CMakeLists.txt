# Master CMakeLists.txt, Author: Alessandro Manzini
# Project configuration specifics.
#
cmake_minimum_required(VERSION 3.26)
project(GameEngine)

# set the C++ standard to C++20 and ensure it is required
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
message(STATUS "C++20 support has been enabled by default.")
# target_compile_features(${TARGET_NAME} PUBLIC cxx_std_20)

if(EMSCRIPTEN)
    message(STATUS "Using Emscripten!")
endif()

# append cmake module path
# making sure cmake finds our custom modules (.cmake)
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_SOURCE_DIR}/cmake")

# set the executable directory
set(GAME_BINARY_PATH "${CMAKE_BINARY_DIR}/game")

# add the engine subdirectory
add_subdirectory(engine)

# add the game subdirectory
add_subdirectory(game)

