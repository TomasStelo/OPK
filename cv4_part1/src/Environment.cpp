#include "../include/Environment.h"

#include <cmath>
#include <stdexcept>

namespace environment {

Environment::Environment(const Config& config)
	: resolution_(config.resolution) {
	if (resolution_ <= 0.0) {
		throw std::invalid_argument("Environment resolution must be positive.");
	}

	map_ = cv::imread(config.map_filename, cv::IMREAD_GRAYSCALE);
	if (map_.empty()) {
		throw std::runtime_error("Failed to load map file: " + config.map_filename);
	}
}

bool Environment::isOccupied(double x, double y) const {
	if (x < 0.0 || y < 0.0) {
		return true;
	}

	const int px = static_cast<int>(std::floor(x / resolution_));
	const int py = static_cast<int>(std::floor(y / resolution_));

	if (px < 0 || py < 0 || px >= map_.cols || py >= map_.rows) {
		return true;
	}

	const unsigned char value = map_.at<unsigned char>(py, px);
	return value == 0;
}

double Environment::getWidth() const {
	return static_cast<double>(map_.cols) * resolution_;
}

double Environment::getHeight() const {
	return static_cast<double>(map_.rows) * resolution_;
}

} // namespace environment
