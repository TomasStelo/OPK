cmake_minimum_required(VERSION 3.10)
project(Cvicenie_4)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)

file(GLOB SOURCES src/*.cpp)

add_executable(Cvicenie_4 ${SOURCES})

target_include_directories(Cvicenie_4 PRIVATE include)

target_link_libraries(Cvicenie_4 ${OpenCV_LIBS})
