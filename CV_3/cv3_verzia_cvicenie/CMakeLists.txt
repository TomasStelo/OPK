cmake_minimum_required(VERSION 3.10) 
project(FifoApp VERSION 1.0.0 LANGUAGES CXX) 
set(CMAKE_CXX_STANDARD 20) 
set(CMAKE_CXX_STANDARD_REQUIRED True) 

add_subdirectory(src/fifo_lib)
add_subdirectory(tests)

add_executable(fifo_app
    main.cpp
)
target_include_directories(fifo_app PRIVATE ${CMAKE_SOURCE_DIR}/include/)
target_link_libraries(fifo_app PRIVATE fifo_lib)

target_compile_options(fifo_app PRIVATE
                      -Wall; -Wextra; -Werror)
    
target_compile_options(fifo_app PRIVATE 
    $<$<CONFIG:Debug>:-fsanitize=undefined>) 
target_link_options(fifo_app PRIVATE 
    $<$<CONFIG:Debug>:-fsanitize=undefined>)          