//
// Created by tomas on 2/25/26.
//

#include "canvas.h"
#include <iostream>
#include <opencv4/opencv2/opencv.hpp>

Canvas::Canvas(int width, int height) : width_(width), height_(height) {
    image_ = cv::Mat(height, width, CV_8UC3, cv::Scalar(255, 255, 255));
}

void Canvas::show() const {
    cv::imshow("Type shiiit", image_);
}

void Canvas::clear() {
    image_ = cv::Mat(height_, width_, CV_8UC3, cv::Scalar(0, 0, 0));
}

cv::Mat& Canvas::getImage() {
    return image_;
}
