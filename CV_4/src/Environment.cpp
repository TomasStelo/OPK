#include "../include/Environment.h"

#include <iostream>

#include <opencv2/opencv.hpp>

namespace environment {

    Environment::Environment(const Config &config) {
        map_ = cv::imread(config.map_filename, cv::IMREAD_GRAYSCALE);
        if (map_.empty()) {
            std::cerr << "Could not open or find the map: " << config.map_filename << std::endl;
            return;
        }

        resolution_ = config.resolution;
        cv::threshold(map_, map_, 127, 255, cv::THRESH_BINARY);
    }

    bool Environment::isOccupied(double x, double y) const {
        int px = static_cast<int>(x / resolution_);
        int py = static_cast<int>(y / resolution_);
        if (px < 0 || px >= map_.cols || py < 0 || py >= map_.rows)
            return true; // mimo mapy = kolízia
        return map_.at<uchar>(py, px) < 128; // tmavé pixely = prekážka
    }

    double Environment::getWidth() const { return map_.cols * resolution_; }
    double Environment::getHeight() const { return map_.rows * resolution_; }

    cv::Mat& Environment::getCanvasRef() { return map_; }

} // namespace environment