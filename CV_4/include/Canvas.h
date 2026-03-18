#pragma once

#include <opencv2/opencv.hpp>
#include <vector>
#include <memory>
#include "types/Geometry.h"

namespace environment {
    class Environment;
}

namespace canvas {

    struct Config {
        double resolution;
    };

    class Canvas {
    public:
        Canvas(const Config& config, const cv::Mat& map,
               std::shared_ptr<environment::Environment> environment = nullptr);

        void drawRobot(const geometry::RobotState& state);
        void drawLidarRays(const geometry::RobotState& state,
                          const std::vector<geometry::Point2d>& points);
        void display(const std::string& window_name = "Map");
        void resetBackground(const cv::Mat& background);

    private:
        Config config_;
        cv::Mat display_;
        std::shared_ptr<environment::Environment> environment_;
    };

} // namespace canvas
