#pragma once

#include <opencv2/opencv.hpp>
#include <string>

namespace canvas {

struct Config {
	std::string map_filename;
	double resolution;
};

class Canvas {
public:
	explicit Canvas(const Config& config);

	bool loadMap();
	void resetBackground();
	void drawPointPixel(int x, int y, const cv::Scalar& color = cv::Scalar(0, 0, 255), int radius = 3);
	void drawLine(int x1, int y1, int x2, int y2, const cv::Scalar& color = cv::Scalar(0, 0, 255), int thickness = 1);
	void drawRobot(int x, int y, const cv::Scalar& color = cv::Scalar(0, 255, 0), int radius = 5);
	void display(const std::string& window_name = "Map") const;

	const cv::Mat& getBackground() const;
	const cv::Mat& getDisplay() const;

private:
	Config config_;
	cv::Mat background_;
	cv::Mat display_;
};

} // namespace canvas
