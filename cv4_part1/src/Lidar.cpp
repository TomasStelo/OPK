#include "../include/Lidar.h"

#include <cmath>

namespace lidar {

Lidar::Lidar(const Config& config, std::shared_ptr<environment::Environment> env)
	: config_(config), env_(std::move(env)) {
}

std::vector<geometry::Point2d> Lidar::scan(const geometry::RobotState& state) const {
	std::vector<geometry::Point2d> hits;
	if (!env_ || config_.beam_count <= 0 || config_.max_range <= 0.0) {
		return hits;
	}

	hits.reserve(static_cast<size_t>(config_.beam_count));

	const double step = 0.02;
	const int beam_count = config_.beam_count;
	const double ray_span = config_.last_ray_angle - config_.first_ray_angle;
	const double beam_step = (beam_count > 1) ? (ray_span / static_cast<double>(beam_count - 1)) : 0.0;

	for (int i = 0; i < beam_count; ++i) {
		const double rel_angle = config_.first_ray_angle + beam_step * static_cast<double>(i);
		const double angle = state.theta + rel_angle;

		geometry::Point2d endpoint{
			state.x + config_.max_range * std::cos(angle),
			state.y + config_.max_range * std::sin(angle)
		};

		for (double range = step; range <= config_.max_range; range += step) {
			const double x = state.x + range * std::cos(angle);
			const double y = state.y + range * std::sin(angle);

			if (env_->isOccupied(x, y)) {
				endpoint = {x, y};
				break;
			}
		}

		hits.push_back(endpoint);
	}

	return hits;
}

} // namespace lidar
