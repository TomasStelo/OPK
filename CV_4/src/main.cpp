#include "../include/Environment.h"
#include "../include/Robot.h"
#include "../include/Lidar.h"
#include "../include/Canvas.h"

#include <memory>

#include <opencv4/opencv2/opencv.hpp>

// Struct pre mouse callback
struct MouseData {
    robot::Robot* robot;
    double resolution;
};

// Mouse callback funkcia
void mouseCallback(int event, int x, int y, int flags, void* userdata) {
    if (event == cv::EVENT_LBUTTONDOWN) {
        MouseData* data = static_cast<MouseData*>(userdata);

        // Konvertuj pixel koordináty na svetové koordináty
        double world_x = x * data->resolution;
        double world_y = y * data->resolution;

        // Nastav robota na novú pozíciu
        data->robot->setPosition(world_x, world_y, 0.0);
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

    robot::Robot my_robot(robot_config);

    // --- Mouse Callback ---
    MouseData mouse_data;
    mouse_data.robot = &my_robot;
    mouse_data.resolution = environment_config.resolution;
    cv::setMouseCallback("Map", mouseCallback, &mouse_data);

    // spawn do stredu mapy
    double spawn_x = environment_ptr->getWidth() / 2.0;
    double spawn_y = environment_ptr->getHeight() / 2.0;
    my_robot.setPosition(spawn_x, spawn_y, 0.0);

    // --- Lidar ---
    lidar::Config lidar_config;
    lidar_config.max_range = 10.0;
    lidar_config.beam_count = 36;
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
        my_robot.update(velocity, robot_config.command_duration);

        auto points = my_lidar.scan(my_robot.getState());
        auto state = my_robot.getState();

        // Canvas vykreslovanie
        my_canvas.resetBackground(environment_ptr->getCanvasRef());
        my_canvas.drawRobot(state);
        my_canvas.drawLidarRays(state, points);
        my_canvas.display("Map");
    }

    cv::destroyAllWindows();
    return 0;
}