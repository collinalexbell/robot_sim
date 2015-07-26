#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_rotozoom.h>
#include "robot.h"
#include "point.h"
#include "modulo.h"
#include "world.h"
#include <iostream>
#include <math.h>

#define PI 3.14159265

Robot::Robot(){
   position_exists = false; 
   current_image = IMG_Load("./resources/ball_bot.png");
   angle = 0;
}

Robot::Robot(std::string dna){
    nnet = new Spiking_NNet(dna);
}

void Robot::set_position(int x, int y){
    position.x = x;
    position.y = y;
    position_exists = true;
}

Point Robot::get_position(){
    if(position_exists){
        return position;
    }
    else{
        throw "Position does not exist";
    }
}

SDL_Surface* Robot::get_image(){
    return current_image;
}


vector<SDL_Surface*> Robot::make_sprites(const char* file){
    int i;
    SDL_Surface* sprite = IMG_Load("./resources/ball_bot.png");
    SDL_Surface* point;
    vector<SDL_Surface*> rv;

    for ( i=0; i<=360; i++ ){
        rv.push_back(rotozoomSurface(sprite, i, 1, NULL));

    }

    return rv;

}

const char* file_name = "./resources/ball_bot.png";
vector<SDL_Surface*> Robot::sprites = Robot::make_sprites(file_name);

void Robot::rotate(int deg){
    angle = modulo((angle + deg), 360);
    current_image = sprites[angle];
    std::cout<<angle<<std::endl;

}

void Robot::move(int distance){
    int delta_x = distance * round(cos(angle*PI/180));

    //I want to use quadrant 1 angles and quadrant 4 positions
    //Fuck graphics coors
    int delta_y = distance * -round(sin(angle*PI/180));
    
    position.x += delta_x;
    position.y += delta_y;

}

void Robot::add_distance_sensor(std::string name, double angle, double width){
    Distance_Sensor * sensor;     

    sensor = new Distance_Sensor(this, angle, width);
    std::pair<std::string, Distance_Sensor*> insert_me (name, sensor);

    distance_sensors.insert(insert_me);
}

double Robot::sense(std::string sensor_name){
    Distance_Sensor *sensor;
    try{
        sensor = distance_sensors.at(sensor_name);
    }
    catch(...){
        throw "Sensor must exist for a robot to sense with it";
    }
    
    try{
        std::vector<Garden*> gardens = world->get_gardens();
        std::vector<Drawable*> drawables(gardens.begin(), gardens.end());
        return sensor->sense(drawables);
    }
    catch(...){
       throw "Robot can ony sense if it is in a world of objects";
    }

}

Spiking_NNet* Robot::get_nnet(){
    return nnet;
}




