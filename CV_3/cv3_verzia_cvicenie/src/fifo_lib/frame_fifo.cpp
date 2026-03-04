#include <memory>
#include <new>

#include "fifo_lib/frame_fifo.hpp"


StatusCode FrameFifo::pushBack(const CameraFrame& frame)
{
  
    auto node = std::make_unique<ListNode>(std::make_shared<const CameraFrame>(frame)); 

    if (!head_)
    {
        head_ = std::move(node);
        tail_ = head_.get();
        size_++;
        return StatusCode::OK;
    }
    
    tail_->next_ = std::move(node);
    tail_ = tail_->next_.get();
    size_++;
 
    return StatusCode::OK;
}


StatusCode FrameFifo::popFront()
{
    if (!head_)
    {
        return StatusCode::ERROR_EMPTY;
    }

    head_ = std::move(head_->next_);
    size_--;
    

    if (!head_)
    {
        tail_ = nullptr;
    }
    
    return StatusCode::OK;
}

bool FrameFifo::empty() const
{
    return head_ == nullptr;
}

int FrameFifo::getSize() const
{
    return size_;
}


std::weak_ptr<const CameraFrame> FrameFifo::at (int index) const
{
    if (index < 0 || index >= size_)
    {
        return {};
    }

    const ListNode* current_node = head_.get();
    for (int i = 0; i < index; i++)
    {
        current_node = current_node->next_.get();
    }

    return std::weak_ptr<const CameraFrame>{current_node->frame_};
}

