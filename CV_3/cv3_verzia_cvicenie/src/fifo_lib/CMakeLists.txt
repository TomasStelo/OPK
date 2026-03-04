add_library(fifo_lib STATIC
    camera_frame.cpp
    frame_fifo.cpp
    list_node.cpp
)

target_include_directories(fifo_lib PUBLIC  ${CMAKE_SOURCE_DIR}/include/)

