cmake_minimum_required(VERSION 3.14)
project(CV_2)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenCV REQUIRED)

add_executable(cv_2 main.cpp canvas.cpp canvas.h)

target_link_libraries(cv_2 ${OpenCV_LIBS})
