#pragma once

#include <string>


namespace environment {

struct Config {
    std::string map_filename;
    double resolution;
};

class Environment {
public:
    explicit Environment(const Config& config);

    bool isOccupied(double x, double y) const;

    double getWidth() const;
    double getHeight() const;
};

} // namespace environment
