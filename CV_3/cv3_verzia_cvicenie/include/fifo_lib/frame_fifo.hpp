#pragma once
#include <memory>

#include "fifo_lib/error_codes.hpp"
#include "fifo_lib/list_node.hpp"
#include "fifo_lib/error_codes.hpp"
#include "fifo_lib/camera_frame.hpp"

class FrameFifo
{
public:
    FrameFifo() = default;
    ~FrameFifo() = default;

    bool empty() const;
    StatusCode pushBack(const CameraFrame& frame);
    StatusCode popFront(); 
    int getSize() const;

    std::weak_ptr<const CameraFrame> at (int index) const;

private:
    std::unique_ptr<ListNode> head_ = nullptr;
    ListNode* tail_ = nullptr;
    int size_ = 0;
};

