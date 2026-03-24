#include "../include/Canvas.h"

namespace canvas {

Canvas::Canvas(const Config& config)
	: config_(config) {
}

bool Canvas::loadMap() {
	background_ = cv::imread(config_.map_filename, cv::IMREAD_COLOR);
	if (background_.empty()) {
		display_.release();
		return false;
	}

	background_.copyTo(display_);
	return true;
}

void Canvas::resetBackground() {
	if (!background_.empty()) {
		background_.copyTo(display_);
	}
}

void Canvas::drawPointPixel(int x, int y, const cv::Scalar& color, int radius) {
	if (display_.empty()) {
		return;
	}

	if (x < 0 || y < 0 || x >= display_.cols || y >= display_.rows) {
		return;
	}

	cv::circle(display_, cv::Point(x, y), radius, color, -1);
}

void Canvas::drawLine(int x1, int y1, int x2, int y2, const cv::Scalar& color, int thickness) {
	if (display_.empty()) {
		return;
	}

	cv::line(display_, cv::Point(x1, y1), cv::Point(x2, y2), color, thickness);
}

void Canvas::drawRobot(int x, int y, const cv::Scalar& color, int radius) {
	if (display_.empty()) {
		return;
	}

	cv::circle(display_, cv::Point(x, y), radius, color, -1);
}

void Canvas::display(const std::string& window_name) const {
	if (!display_.empty()) {
		cv::imshow(window_name, display_);
	}
}

const cv::Mat& Canvas::getBackground() const {
	return background_;
}

const cv::Mat& Canvas::getDisplay() const {
	return display_;
}

} // namespace canvas
