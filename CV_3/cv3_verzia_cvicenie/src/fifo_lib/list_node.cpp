#include <memory>
#include <iostream>

#include "fifo_lib/list_node.hpp"

ListNode::ListNode(std::shared_ptr<const CameraFrame> f) noexcept : frame_(std::move(f))
{
    std::cout << "ListNode constructor" << std::endl;
}

ListNode::~ListNode()
{
    std::cout << "ListNode destructor" << std::endl;
}
