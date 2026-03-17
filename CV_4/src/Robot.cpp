#include "../include/Robot.h"
#include <cmath>
#include <iostream>

namespace robot {

    Robot::Robot(const Config &config, const CollisionCb &collision_cb)
        : config_(config), collision_cb_(collision_cb)
    {
        state_.x = 0.0;
        state_.y = 0.0;
        state_.theta = 0.0;
        state_.velocity = {0.0, 0.0};
    }

    Robot::~Robot() {}

    void Robot::setVelocity(const geometry::Twist &velocity) {
        state_.velocity = velocity;
    }

    geometry::RobotState Robot::getState() const {
        return state_;
    }

    bool Robot::isInCollision() const {
        if (collision_cb_) {
            return collision_cb_(state_);
        }
        return false;
    }

    void Robot::update(const geometry::Twist &velocity, double dt) {
        state_.x += velocity.linear * std::cos(state_.theta) * dt;
        state_.y += velocity.linear * std::sin(state_.theta) * dt;
        state_.theta += velocity.angular * dt;  // rotácia teraz funguje
        state_.velocity = velocity;
    }

    // nové: nastavenie počiatočnej pozície
    void Robot::setPosition(double x, double y, double theta) {
        state_.x = x;
        state_.y = y;
        state_.theta = theta;
        state_.velocity = {0.0, 0.0};
    }

} // namespace robot