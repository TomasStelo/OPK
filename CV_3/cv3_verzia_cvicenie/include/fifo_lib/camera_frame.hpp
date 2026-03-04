#pragma once


class CameraFrame
{
public:
    CameraFrame();
    ~CameraFrame();

    CameraFrame(const CameraFrame& other);
    CameraFrame(CameraFrame&& other) noexcept;
    CameraFrame& operator=(const CameraFrame& other);
    CameraFrame& operator=(CameraFrame&& other) noexcept;

    int getStride() const;
    int getLength() const;

    const uint8_t* getData() const;

private:
    int stride_ = 0;
    int length_ = 0;
    std::unique_ptr<uint8_t[]> data_ = nullptr;


    constexpr static int WIDTH = 640;
    constexpr static int HEIGHT = 480;

}; 
