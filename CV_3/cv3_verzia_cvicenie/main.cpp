#include <iostream>

#include "fifo_lib/frame_fifo.hpp"

int main()
{
    FrameFifo fifo;

    for (int i = 0; i < 5; i++)
    {;
        CameraFrame frame;
        fifo.pushBack(frame);
        std::cout << "Pushed frame " << i << std::endl;
    }

    std::cout << "Size of FIFO: " << fifo.getSize() << std::endl << std::endl;

    for (int i = 0; i < 5; i++)
    {
        auto frame = fifo.at(i).lock();
        if (frame)
        {
            std::cout << "Frame " << i << ": Stride = " << frame->getStride() 
                      << ", Length = " << frame->getLength() << std::endl;
        }
    }

    std::cout << std::endl;

    while (!fifo.empty())
    {
        fifo.popFront();
        std::cout << std::endl;
    }

    std::cout << "Size of FIFO after popping: " << fifo.getSize() << std::endl;

    return 0;
}

