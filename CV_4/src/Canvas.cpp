#include "../include/Canvas.h"
#include "../include/Environment.h"

namespace canvas {

    Canvas::Canvas(const Config& config, const cv::Mat& map,
                   std::shared_ptr<environment::Environment> environment)
        : config_(config), environment_(environment)
    {
        // Konvertuj grayscale na BGR pre správne farby
        if (map.channels() == 1) {
            cv::cvtColor(map, display_, cv::COLOR_GRAY2BGR);
        } else {
            map.copyTo(display_);
        }
    }

    void Canvas::drawRobot(const geometry::RobotState& state)
    {
        int robot_x = static_cast<int>(state.x / config_.resolution);
        int robot_y = static_cast<int>(state.y / config_.resolution);
        cv::circle(display_, cv::Point(robot_x, robot_y), 5, cv::Scalar(0, 255, 0), -1);  // Zelený
    }

    void Canvas::drawLidarRays(const geometry::RobotState& state,
                               const std::vector<geometry::Point2d>& points)
    {
        int robot_x = static_cast<int>(state.x / config_.resolution);
        int robot_y = static_cast<int>(state.y / config_.resolution);

        for (const auto& p : points) {
            int px = static_cast<int>(p.x / config_.resolution);
            int py = static_cast<int>(p.y / config_.resolution);
            cv::line(display_, cv::Point(robot_x, robot_y), cv::Point(px, py),
                     cv::Scalar(180, 180, 180), 1);  // Svetlejšie sivé

            // Vykresli bodku len ak sa lúč dotkol steny
            if (environment_ && environment_->isOccupied(p.x, p.y)) {
                cv::circle(display_, cv::Point(px, py), 2, cv::Scalar(200, 200, 200), -1);
            }
        }
    }

    void Canvas::display(const std::string& window_name)
    {
        cv::imshow(window_name, display_);
    }

    void Canvas::resetBackground(const cv::Mat& background)
    {
        // Konvertuj grayscale na BGR pre správne farby
        if (background.channels() == 1) {
            cv::cvtColor(background, display_, cv::COLOR_GRAY2BGR);
        } else {
            background.copyTo(display_);
        }
    }

} // namespace canvas
