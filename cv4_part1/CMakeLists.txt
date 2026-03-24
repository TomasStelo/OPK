cmake_minimum_required(VERSION 3.10)
project(cv4_part1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

add_executable(cv4_part1
    src/main.cpp
    src/Canvas.cpp
    src/Environment.cpp
    src/Lidar.cpp
)

target_include_directories(cv4_part1 PRIVATE include)
target_link_libraries(cv4_part1 PRIVATE ${OpenCV_LIBS})

# Unit testy
enable_testing()

add_executable(cv4_tests
    tests/robot_test.cpp
    src/Environment.cpp
    src/Lidar.cpp
)

target_include_directories(cv4_tests PRIVATE include)
target_link_libraries(cv4_tests PRIVATE ${OpenCV_LIBS} GTest::gtest GTest::gtest_main)

add_test(NAME cv4_tests COMMAND cv4_tests)

