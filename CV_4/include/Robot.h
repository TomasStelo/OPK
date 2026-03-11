#pragma once

#include <functional>
#include "types/Geometry.h"

namespace robot {

struct Config {
    geometry::Twist accelerations;
    geometry::Twist emergency_decelerations;
    double command_duration;
    int simulation_period_ms;
};


class Robot {
public:
    using CollisionCb = std::function<bool(geometry::RobotState)>;

    Robot(const Config& config, const CollisionCb& collision_cb = nullptr);
    ~Robot();
    void setVelocity(const geometry::Twist& velocity);
    geometry::RobotState getState() const;
    bool isInCollision() const;
protected:
    void update(const geometry::Twist& velocity, double dt);

};
} // namespace robot

