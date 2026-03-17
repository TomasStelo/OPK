# pragma once

namespace geometry {

struct Point2d {
    double x;
    double y;
};

struct Twist {
    double linear;
    double angular;
};


struct RobotState {
    double x;
    double y;
    double theta;
    Twist velocity;
};

} // namespace geometry

