//
// Created by tomas on 2/25/26.
//

#ifndef UNTITLED_CANVAS_H
#define UNTITLED_CANVAS_H

#include <opencv4/opencv2/opencv.hpp>

class Canvas {
    public:
        Canvas(int width, int height);
        void show() const;
        void clear();
        cv::Mat& getImage();
    private:
        cv::Mat image_;
        int width_;
        int height_;
};


#endif //UNTITLED_CANVAS_H