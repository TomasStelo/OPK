#include "../include/Canvas.h"
#include "../include/Environment.h"
#include "../include/Lidar.h"
#include "../include/Geometry.h"

#include <cmath>
#include <iostream>
#include <vector>

struct MouseData {
	std::vector<cv::Point>* points;
};

void mouseCallback(int event, int x, int y, int, void* userdata) {
	if (event != cv::EVENT_LBUTTONDOWN || userdata == nullptr) {
		return;
	}

	auto* data = static_cast<MouseData*>(userdata);
	data->points->emplace_back(x, y);
}

int main() {
	canvas::Config config;
	config.map_filename = "../opk-map.png";
	config.resolution = 0.05;

	environment::Config env_config;
	env_config.map_filename = config.map_filename;
	env_config.resolution = config.resolution;
	auto environment = std::make_shared<environment::Environment>(env_config);

	constexpr double kPi = 3.14159265358979323846;
	lidar::Config lidar_config;
	lidar_config.max_range = 10.0;
	lidar_config.beam_count = 60;
	lidar_config.first_ray_angle = -kPi;
	lidar_config.last_ray_angle = kPi;
	lidar::Lidar lidar(lidar_config, environment);

	canvas::Canvas map_canvas(config);
	if (!map_canvas.loadMap()) {
		std::cerr << "Nepodarilo sa nacitat mapu: " << config.map_filename << '\n';
		return 1;
	}

	cv::namedWindow("Map", cv::WINDOW_AUTOSIZE);
	std::vector<cv::Point> clicked_points;
	MouseData mouse_data{&clicked_points};
	cv::setMouseCallback("Map", mouseCallback, &mouse_data);

	bool running = true;
	while (running) {
		map_canvas.resetBackground();
		for (const auto& p : clicked_points) {
			// Vykresliť robota - zelený kruh
			map_canvas.drawRobot(p.x, p.y, cv::Scalar(0, 255, 0), 5);

			geometry::RobotState state{};
			state.x = static_cast<double>(p.x) * config.resolution;
			state.y = static_cast<double>(p.y) * config.resolution;
			state.theta = 0.0;
			state.velocity = {0.0, 0.0};

			const auto hits = lidar.scan(state);
			for (const auto& hit : hits) {
				const int hx = static_cast<int>(std::lround(hit.x / config.resolution));
				const int hy = static_cast<int>(std::lround(hit.y / config.resolution));

				// Vykresliť čiaru (lidar lúč)
				map_canvas.drawLine(p.x, p.y, hx, hy, cv::Scalar(128, 128, 128), 1);

				// Vykresliť bodku na konci lúča
				map_canvas.drawPointPixel(hx, hy, cv::Scalar(128, 128, 128), 1);
			}
		}

		// Vykresliť zelené bodky v stredoch robotov na koniec, aby boli viditeľné
		for (const auto& p : clicked_points) {
			map_canvas.drawPointPixel(p.x, p.y, cv::Scalar(0, 255, 0), 2);
		}

		map_canvas.display("Map");
		const int key = cv::waitKey(30);
		if (key == 27) {
			running = false;
		}
	}

	cv::destroyAllWindows();
	return 0;
}
