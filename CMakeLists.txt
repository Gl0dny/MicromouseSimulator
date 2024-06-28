cmake_minimum_required(VERSION 3.10)

# Project name
project(MicromouseProject)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set the debug flag based on the build type
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DDEBUG_MODE)
endif()

# include(FetchContent)
# FetchContent_Declare(
#   googletest
#   URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
# )

# For Windows: Prevent overriding the parent project's compiler/linker settings
# set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
# FetchContent_MakeAvailable(googletest)

# Włączenie automatycznego generowania plików MOC, RCC i UIC dla Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# If using Qt5, uncomment these lines and add the necessary components
find_package(Qt5 REQUIRED COMPONENTS Widgets)
include_directories(${Qt5Widgets_INCLUDE_DIRS})

# Include directories
include_directories(include)

# Source files
set(SOURCES
    src/Maze.cpp
    src/Logger.cpp
    src/Utils.cpp
    src/Micromouse.cpp
    src/Sensor.cpp
    src/Simulator.cpp
)

# Main executable
add_executable(MicromouseProject main.cpp ${SOURCES})

# Linkowanie bibliotek Qt5
target_link_libraries(MicromouseProject Qt5::Widgets)

# Enable testing
enable_testing()

# Google Test
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Test executable
add_executable(tests tests/test_maze.cpp ${SOURCES})

# Link pthread for threads
target_link_libraries(MicromouseProject pthread)

# Linkowanie bibliotek Qt5 i Google Test do testów
target_link_libraries(tests Qt5::Widgets GTest::GTest GTest::Main)

# Add the tests
add_test(NAME maze_tests COMMAND tests)
