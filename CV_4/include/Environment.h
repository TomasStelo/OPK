#pragma once

#include <string>
#include <opencv2/opencv.hpp>

namespace environment {

    struct Config {
        std::string map_filename;
        double resolution;
    };

    class Environment {
    public:
        explicit Environment(const Config& config);

        bool isOccupied(double x, double y) const;
        double getWidth() const;
        double getHeight() const;

        // nové: vráti referenciu na hlavnú mapu, aby sme nemuseli kopírovať
        cv::Mat& getCanvasRef();

    private:
        cv::Mat map_;
        double resolution_;
    };

} // namespace environment