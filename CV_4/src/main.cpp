#include "../include/Environment.h"
#include "../include/Robot.h"
#include "../include/Lidar.h"
#include "../include/Canvas.h"

#include <memory>
#include <vector>

#include <opencv4/opencv2/opencv.hpp>

// Struct pre mouse callback
struct MouseData {
    std::vector<std::unique_ptr<robot::Robot>>* robots;
    double resolution;
    robot::Config* robot_config;
};

// Mouse callback funkcia
void mouseCallback(int event, int x, int y, int flags, void* userdata) {
    if (event == cv::EVENT_LBUTTONDOWN) {
        MouseData* data = static_cast<MouseData*>(userdata);

        // Konvertuj pixel koordináty na svetové koordináty
        double world_x = x * data->resolution;
        double world_y = y * data->resolution;

        // Vytvor nového robota
        auto new_robot = std::make_unique<robot::Robot>(*data->robot_config);
        new_robot->setPosition(world_x, world_y, 0.0);
        data->robots->push_back(std::move(new_robot));
    }
}

int main() {
    // --- Environment ---
    environment::Config environment_config;
    environment_config.map_filename = "../opk-map.png";
    environment_config.resolution = 0.05;
    std::shared_ptr<environment::Environment> environment_ptr = std::make_shared<environment::Environment>(environment_config);

    // --- Canvas ---
    canvas::Config canvas_config;
    canvas_config.resolution = environment_config.resolution;
    canvas::Canvas my_canvas(canvas_config, environment_ptr->getCanvasRef(), environment_ptr);
    cv::namedWindow("Map", cv::WINDOW_NORMAL);

    // --- Robot ---
    robot::Config robot_config;
    robot_config.command_duration = 0.1;
    robot_config.simulation_period_ms = 50;

    std::vector<std::unique_ptr<robot::Robot>> robots;

    // Vytvor prvého robota
    auto first_robot = std::make_unique<robot::Robot>(robot_config);
    double spawn_x = environment_ptr->getWidth() / 2.0;
    double spawn_y = environment_ptr->getHeight() / 2.0;
    first_robot->setPosition(spawn_x, spawn_y, 0.0);
    robots.push_back(std::move(first_robot));

    // --- Mouse Callback ---
    MouseData mouse_data;
    mouse_data.robots = &robots;
    mouse_data.resolution = environment_config.resolution;
    mouse_data.robot_config = &robot_config;
    cv::setMouseCallback("Map", mouseCallback, &mouse_data);


    // --- Lidar ---
    lidar::Config lidar_config;
    lidar_config.max_range = 10.0;
    lidar_config.beam_count = 36; //36
    lidar_config.first_ray_angle = -3.14159 / 2;
    lidar_config.last_ray_angle = 3.14159 / 2;
    lidar::Lidar my_lidar(lidar_config, environment_ptr);

    // --- Loop ---
    bool running = true;
    while (running) {
        int key = cv::waitKey(robot_config.simulation_period_ms) & 0xFF;

        geometry::Twist velocity{0.0, 0.0};
        switch (key) {
            case 'w': velocity.linear = 0.5; break;
            case 's': velocity.linear = -0.5; break;
            case 'a': velocity.angular = -0.5; break;
            case 'd': velocity.angular = 0.5; break;
            case 27: running = false; break;
        }

        // Updatuj len posledného robota (ten najnovšie spawnutý)
        if (!robots.empty()) {
            robots.back()->update(velocity, robot_config.command_duration);
        }

        // Vykresli všetkých robotov
        my_canvas.resetBackground(environment_ptr->getCanvasRef());
        for (auto& robot : robots) {
            auto state = robot->getState();
            auto points = my_lidar.scan(state);

            my_canvas.drawRobot(state);
            my_canvas.drawLidarRays(state, points);
        }
        my_canvas.display("Map");
    }

    cv::destroyAllWindows();
    return 0;
}