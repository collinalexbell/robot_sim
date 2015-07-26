#ifndef DISTANCE_SENSOR_H
#define DISTANCE_SENSOR_H



#include <vector>
#include "agent.h"

class Distance_Sensor{
    public:
        Distance_Sensor(Agent*, double, double);
        bool angle_in_range(double angle, double a, double b);
        double sense(std::vector<Drawable*>);
    private:
        Agent* owner;
        double center_angle;
        double sensor_width;

};

#endif
