#ifndef DISTANCE_SENSOR_H
#define DISTANCE_SENSOR_H

#include <vector>
#include "robot.h"

class Distance_Sensor{
    public:
        Distance_Sensor(Robot*, double, double);
        bool angle_in_range(double angle, double a, double b);
        double sense(std::vector<Drawable*>);
    private:
        Robot* owner;
        double center_angle;
        double sensor_width;

};

#endif
