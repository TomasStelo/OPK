#include <iostream>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <algorithm>

#include "fifo_lib/camera_frame.hpp"


CameraFrame::CameraFrame() : stride_(WIDTH), length_(stride_ * HEIGHT)
{
    std::cout << "CameraFrame constructor" << std::endl;
    //Strided array allocated on heap, owned by this object
    data_ = std::make_unique<uint8_t[]>(length_);
    
    //Filling some sample data
    for (int i = 0; i < length_; i++)
    {
        data_[i] = i % 256;
    }
}

CameraFrame::~CameraFrame()
{
    std::cout << "CameraFrame destructor" << std::endl;
}


CameraFrame::CameraFrame(const CameraFrame& other)
{
    std::cout << "CameraFrame copy constructor" << std::endl;

    if (other.data_!=nullptr && other.length_ > 0) 
    {
        stride_ = other.stride_;
        length_ = other.length_;

        data_ = std::make_unique<uint8_t[]>(length_);
        std::copy_n(other.data_.get(), length_, data_.get());

    }
    else 
    {
        //Here exception would be thrown, but we don't have them yet
        std::cerr << "CameraFrame copy constructor: other is empty" << std::endl;
        data_ = nullptr;
        stride_ = 0;
        length_ = 0;
    }

}

CameraFrame::CameraFrame(CameraFrame&& other) noexcept
{
    std::cout << "CameraFrame move constructor" << std::endl;
    //moving data from other to this object
    data_ = std::move(other.data_);
    stride_ = other.stride_;
    length_ = other.length_;
    // Emptying other object
    other.data_ = nullptr;
    other.stride_ = 0;
    other.length_ = 0;
}

CameraFrame& CameraFrame::operator=(const CameraFrame& other)
{
    if (this == &other) 
    {
        return *this;
    }
    
    if (other.data_ != nullptr && other.length_ > 0)
    {
        stride_ = other.stride_;
        length_ = other.length_;
        data_ = std::make_unique<uint8_t[]>(length_);   
        std::copy_n(other.data_.get(), length_, data_.get());
     
    }
    else
    {
        std::cerr << "CameraFrame copy assignment: other is empty" << std::endl;
        data_ = nullptr;
        stride_ = 0;
        length_ = 0;
    }

    return *this;
}

CameraFrame& CameraFrame::operator=(CameraFrame&& other) noexcept
{
    if (this != &other)
    {
        data_ = std::move(other.data_);
        stride_ = other.stride_;
        length_ = other.length_;
    }
    other.data_ = nullptr;
    other.stride_ = 0;
    other.length_ = 0;

    return *this;
}

int CameraFrame::getStride() const
{
    return stride_;
}

int CameraFrame::getLength() const
{
    return length_;
}

const uint8_t* CameraFrame::getData() const
{
    return data_.get();
}

