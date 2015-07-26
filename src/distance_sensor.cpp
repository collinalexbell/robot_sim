#include "robot.h"
#include "distance_sensor.h"
#include "drawable.h"
#include "modulo.h"
#include <vector>
#include <math.h>

#define WORLD_X 1080
#define WORLD_Y 720
#define PI 3.14159265

Distance_Sensor::Distance_Sensor(Agent *r, double center_ang, double sensor_w){
    owner = r;
    center_angle = center_ang;
    sensor_width = sensor_w;
}

double Distance_Sensor::sense(std::vector<Drawable*> things_to_sense){
    int x_dist, y_dist;
    double thing_to_sense_angle, a, b, rv, angle_offset;
    int robot_x, robot_y, thing_x, thing_y;
    Point pos;

    rv = 0;

    pos = owner->get_position();
    robot_x = pos.x;
    robot_y = pos.y;

    a = modulo(owner->get_angle() - (sensor_width / 2.0) + center_angle, 360);
    b = modulo(owner->get_angle() + (sensor_width / 2.0) + center_angle, 360);

    angle_offset = owner->get_angle();
    for( auto it = things_to_sense.begin(); it != things_to_sense.end(); it++ ){
        pos = (*it)->get_position();
        thing_x = pos.x;
        thing_y = pos.y;

        //Due to Inverted y
        y_dist = robot_y - thing_y;
        x_dist = thing_x - robot_x;

        thing_to_sense_angle = atan ((y_dist)/(x_dist)) * 180 / PI; 

        //Test if it is sensed by sensor
        if(angle_in_range(thing_to_sense_angle, a, b)){
            rv += (double(WORLD_X) -  (sqrt(pow(x_dist,2 ) +  pow(y_dist,2)))) / double(WORLD_X);
        }
    }

    return rv;
}

bool Distance_Sensor::angle_in_range(double angle, double a, double b){
    a = modulo(a, 360);
    b = modulo(b, 360);
    angle = modulo(angle, 360);
    if (a > b){
        if( angle >= a || angle <= b ){
            return true;
        }
    }else{
        if( angle >= a && angle <= b){
            return true;
        }
    }

    return false;

}
