#include <gtest/gtest.h>
#include "../include/Environment.h"
#include "../include/Lidar.h"
#include "../include/Geometry.h"
#include <cmath>
#include <memory>
#include <stdexcept>

// ============================================================================
// Environment Tests
// ============================================================================

// Test načítavania neexistujúceho súboru
TEST(EnvironmentTest, LoadNonExistentFile) {
    environment::Config config;
    config.map_filename = "nonexistent_map.png";
    config.resolution = 0.05;

    EXPECT_THROW({
        environment::Environment env(config);
    }, std::runtime_error);
}

// Test neplatného rozlíšenia
TEST(EnvironmentTest, InvalidResolution) {
    environment::Config config;
    config.map_filename = "../opk-map.png";
    config.resolution = -0.05;

    EXPECT_THROW({
        environment::Environment env(config);
    }, std::invalid_argument);
}

// Test súradnice mimo mapy
TEST(EnvironmentTest, OccupiedOutsideMap) {
    environment::Config config;
    config.map_filename = "../opk-map.png";
    config.resolution = 0.05;

    environment::Environment env(config);

    // Negatívne súradnice by mali byť okupované
    EXPECT_TRUE(env.isOccupied(-1.0, -1.0));

    // Súradnice mimo mapy by mali byť okupované
    double width = env.getWidth();
    double height = env.getHeight();
    EXPECT_TRUE(env.isOccupied(width + 1.0, height + 1.0));
}

// Test rozmerov mapy
TEST(EnvironmentTest, MapDimensions) {
    environment::Config config;
    config.map_filename = "../opk-map.png";
    config.resolution = 0.05;

    environment::Environment env(config);

    double width = env.getWidth();
    double height = env.getHeight();

    // Rozmery by mali byť kladné
    EXPECT_GT(width, 0.0);
    EXPECT_GT(height, 0.0);
}

// Test zdarma miesta (biely pixel = 255)
TEST(EnvironmentTest, FreeSpace) {
    environment::Config config;
    config.map_filename = "../opk-map.png";
    config.resolution = 0.05;

    environment::Environment env(config);

    // Vyskúšame niektoré súradnice, ktoré by mali byť voľné
    // (závisí od mapy, ale aspoň raz by malo byť voľné miesto)
    bool found_free_space = false;
    for (double x = 0.0; x < env.getWidth(); x += 0.5) {
        for (double y = 0.0; y < env.getHeight(); y += 0.5) {
            if (!env.isOccupied(x, y)) {
                found_free_space = true;
                break;
            }
        }
        if (found_free_space) break;
    }

    EXPECT_TRUE(found_free_space);
}

// ============================================================================
// Lidar Tests
// ============================================================================

// Test inicializácie Lidar-u
TEST(LidarTest, LidarInitialization) {
    environment::Config env_config;
    env_config.map_filename = "../opk-map.png";
    env_config.resolution = 0.05;
    auto env = std::make_shared<environment::Environment>(env_config);

    lidar::Config config;
    config.max_range = 10.0;
    config.beam_count = 60;
    config.first_ray_angle = -3.14159;
    config.last_ray_angle = 3.14159;

    lidar::Lidar lidar(config, env);
    EXPECT_TRUE(true);
}

// Test skenu s robotom v strede
TEST(LidarTest, ScanFromCenter) {
    environment::Config env_config;
    env_config.map_filename = "../opk-map.png";
    env_config.resolution = 0.05;
    auto env = std::make_shared<environment::Environment>(env_config);

    lidar::Config config;
    config.max_range = 10.0;
    config.beam_count = 60;
    config.first_ray_angle = -3.14159;
    config.last_ray_angle = 3.14159;

    lidar::Lidar lidar(config, env);

    geometry::RobotState state;
    state.x = 5.0;
    state.y = 5.0;
    state.theta = 0.0;
    state.velocity = {0.0, 0.0};

    auto hits = lidar.scan(state);

    // Počet bodov by mal zodpovedať počtu lúčov
    EXPECT_EQ(hits.size(), static_cast<size_t>(config.beam_count));
}

// Test počtu lúčov
TEST(LidarTest, BeamCount) {
    environment::Config env_config;
    env_config.map_filename = "../opk-map.png";
    env_config.resolution = 0.05;
    auto env = std::make_shared<environment::Environment>(env_config);

    lidar::Config config;
    config.max_range = 10.0;
    config.beam_count = 30;
    config.first_ray_angle = -3.14159;
    config.last_ray_angle = 3.14159;

    lidar::Lidar lidar(config, env);

    geometry::RobotState state;
    state.x = 2.0;
    state.y = 2.0;
    state.theta = 0.0;
    state.velocity = {0.0, 0.0};

    auto hits = lidar.scan(state);
    EXPECT_EQ(hits.size(), static_cast<size_t>(config.beam_count));
}

// Test že body sú v rozsahu max_range
TEST(LidarTest, HitsWithinMaxRange) {
    environment::Config env_config;
    env_config.map_filename = "../opk-map.png";
    env_config.resolution = 0.05;
    auto env = std::make_shared<environment::Environment>(env_config);

    lidar::Config config;
    config.max_range = 5.0;
    config.beam_count = 16;
    config.first_ray_angle = -3.14159;
    config.last_ray_angle = 3.14159;

    lidar::Lidar lidar(config, env);

    geometry::RobotState state;
    state.x = 3.0;
    state.y = 3.0;
    state.theta = 0.0;
    state.velocity = {0.0, 0.0};

    auto hits = lidar.scan(state);

    for (const auto& hit : hits) {
        double distance = std::sqrt(
            (hit.x - state.x) * (hit.x - state.x) +
            (hit.y - state.y) * (hit.y - state.y)
        );
        EXPECT_LE(distance, config.max_range + 0.1);
    }
}

// Test s NULL prostredím
TEST(LidarTest, ScanWithNullEnvironment) {
    lidar::Config config;
    config.max_range = 10.0;
    config.beam_count = 60;
    config.first_ray_angle = -3.14159;
    config.last_ray_angle = 3.14159;

    lidar::Lidar lidar(config, nullptr);

    geometry::RobotState state;
    state.x = 5.0;
    state.y = 5.0;
    state.theta = 0.0;
    state.velocity = {0.0, 0.0};

    auto hits = lidar.scan(state);

    // Bez prostredia by mal vratiť prázdny vektor
    EXPECT_EQ(hits.size(), 0);
}

// Test s rôznymi uhlami robota
TEST(LidarTest, ScanWithDifferentAngles) {
    environment::Config env_config;
    env_config.map_filename = "../opk-map.png";
    env_config.resolution = 0.05;
    auto env = std::make_shared<environment::Environment>(env_config);

    lidar::Config config;
    config.max_range = 10.0;
    config.beam_count = 16;
    config.first_ray_angle = -3.14159;
    config.last_ray_angle = 3.14159;

    lidar::Lidar lidar(config, env);

    geometry::RobotState state;
    state.x = 3.0;
    state.y = 3.0;
    state.velocity = {0.0, 0.0};

    // Test s rôznymi uhlami
    for (double theta = 0.0; theta < 6.28; theta += 1.57) {
        state.theta = theta;
        auto hits = lidar.scan(state);
        EXPECT_EQ(hits.size(), static_cast<size_t>(config.beam_count));
    }
}

