#pragma once
#include "fifo_lib/camera_frame.hpp"

class ListNode
{
public:
    ListNode(std::shared_ptr<const CameraFrame> f) noexcept;
    ~ListNode();

    std::shared_ptr<const CameraFrame> frame_;
    std::unique_ptr<ListNode> next_ = nullptr;   
};

