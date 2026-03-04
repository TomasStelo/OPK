#include <gtest/gtest.h>
#include "fifo_lib/frame_fifo.hpp"

TEST(CameraFIFO, NewFrameFIFOIsEmpty)
{
    FrameFifo fifo;

    EXPECT_TRUE(fifo.empty());
    EXPECT_EQ(fifo.getSize(), 0);
}

TEST(FrameFifoBlackBox, PushBackTwiceSizeTwo) {
    FrameFifo fifo;

    CameraFrame frame1;
    CameraFrame frame2;

    fifo.pushBack(frame1);
    fifo.pushBack(frame2);

    EXPECT_EQ(fifo.getSize(), 2);

    auto f0 = fifo.at(0).lock();
    auto f1 = fifo.at(1).lock();

    ASSERT_NE(f0, nullptr);
    ASSERT_NE(f1, nullptr);
    EXPECT_NE(f0.get(), f1.get());
}


