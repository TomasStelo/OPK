#include "../include/Environment.h"
#include "../include/Robot.h"
#include "../include/Lidar.h"
#include <memory>
#include <opencv4/opencv2/opencv.hpp>

int main() {
    // --- Environment ---
    environment::Config environment_config;
    environment_config.map_filename = "../opk-map.png";
    environment_config.resolution = 0.05;
    std::shared_ptr<environment::Environment> environment_ptr = std::make_shared<environment::Environment>(environment_config);

    cv::Mat& canvas = environment_ptr->getCanvasRef(); // hlavný canvas
    cv::namedWindow("Map", cv::WINDOW_NORMAL);

    // --- Robot ---
    robot::Config robot_config;
    robot_config.command_duration = 0.1;
    robot_config.simulation_period_ms = 50;

    robot::Robot my_robot(robot_config);

    // spawn do stredu mapy
    double spawn_x = environment_ptr->getWidth() / 2.0;
    double spawn_y = environment_ptr->getHeight() / 2.0;
    my_robot.setPosition(spawn_x, spawn_y, 0.0);

    // --- Lidar ---
    lidar::Config lidar_config;
    lidar_config.max_range = 5.0;
    lidar_config.beam_count = 36;
    lidar_config.first_ray_angle = -3.14159;
    lidar_config.last_ray_angle = 3.14159;
    lidar::Lidar my_lidar(lidar_config, environment_ptr);

    // --- Loop ---
    bool running = true;
    while (running) {
        int key = cv::waitKey(robot_config.simulation_period_ms) & 0xFF;

        geometry::Twist velocity{0.0, 0.0};
        switch (key) {
            case 'w': velocity.linear = 0.5; break;
            case 's': velocity.linear = -0.5; break;
            case 'a': velocity.angular = -0.5; break;   // zvýšená rotácia
            case 'd': velocity.angular = 0.5; break;
            case 27: running = false; break;
        }
        my_robot.update(velocity, robot_config.command_duration);

        auto points = my_lidar.scan(my_robot.getState());

        // vykreslenie priamo do hlavného canvasu
        cv::Mat display;
        canvas.copyTo(display);

        auto state = my_robot.getState();
        int robot_x = static_cast<int>(state.x / environment_config.resolution);
        int robot_y = static_cast<int>(state.y / environment_config.resolution);

        cv::circle(display, cv::Point(robot_x, robot_y), 5, cv::Scalar(0, 0, 255), -1);

        for (auto& p : points) {
            int px = static_cast<int>(p.x / environment_config.resolution);
            int py = static_cast<int>(p.y / environment_config.resolution);
            // Zelené Lidar lúče
            cv::line(display, cv::Point(robot_x, robot_y), cv::Point(px, py), cv::Scalar(0, 255, 0), 1);
        }

        cv::imshow("Map", display);
    }

    cv::destroyAllWindows();
    return 0;
}