#include "../include/Lidar.h"
#include <cmath>
#include <iostream>
#include <memory>
#include "../include/types/Geometry.h"

namespace lidar {

    Lidar::Lidar(const Config& config, std::shared_ptr<environment::Environment> environment)
        : config_(config), environment_(environment)
    {
    }

    std::vector<geometry::Point2d> Lidar::scan(const geometry::RobotState& state) const
    {
        std::vector<geometry::Point2d> points;

        double angle_step = (config_.last_ray_angle - config_.first_ray_angle) / (config_.beam_count - 1);

        for (int i = 0; i < config_.beam_count; ++i) {
            double ray_angle = state.theta + config_.first_ray_angle + i * angle_step;

            double x = state.x;
            double y = state.y;
            double step = environment_->getWidth() / 1000.0;
            double dist = 0.0;

            while (dist < config_.max_range) {
                x += step * cos(ray_angle);
                y += step * sin(ray_angle);
                dist += step;

                if (environment_->isOccupied(x, y)) {
                    break;
                }
            }

            points.push_back({x, y});
        }

        return points;
    }

} // namespace lidar