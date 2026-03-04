cmake_minimum_required(VERSION 3.10)

include(GoogleTest)
include(FetchContent)

FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

FetchContent_MakeAvailable(googletest)

add_executable(fifo_tests
        tests.cpp
)

target_link_libraries(fifo_tests
        PRIVATE
        fifo_lib
        GTest::gtest
        GTest::gtest_main
)

gtest_discover_tests(fifo_tests)



