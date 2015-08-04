#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_rotozoom.h>
#include "robot.h"
#include "point.h"
#include "modulo.h"
#include "world.h"
#include <iostream>
#include <math.h>
#include <string>

#define PI 3.14159265

Robot::Robot(){
    init();
    
}

Robot::Robot(std::string dna){
    init();
    nnet = new Spiking_NNet(dna);
}

void Robot::init(){
    SDL_Surface* tmp;
    
    position_exists = false; 
    tmp = IMG_Load("./resources/ball_bot.png");
    current_image = SDL_DisplayFormatAlpha(tmp);
    SDL_FreeSurface(tmp);
    angle = 0;
    nnet = NULL;
    num_of_fruit = 0;
    fruit_cap = 3;
    money = 0;
}

void Robot::set_position(int x, int y){
    position.x = x;
    position.y = y;
    percise_x = x;
    percise_y = y;
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
    SDL_Surface *tmp1, *tmp2, *sprite;
    sprite  = IMG_Load("./resources/ball_bot.png");
    SDL_Surface* point;
    vector<SDL_Surface*> rv;

    for ( i=0; i<=360; i++ ){
        tmp1 = rotozoomSurface(sprite, i, 1, NULL);
        //tmp2 = SDL_DisplayFormatAlpha(tmp1);
        rv.push_back(tmp1);
        SDL_FreeSurface(tmp1);
    }
    SDL_FreeSurface(sprite);

    return rv;

}

void Robot::give_fruit(){
    num_of_fruit ++;
}

const char* file_name = "./resources/ball_bot.png";
vector<SDL_Surface*> Robot::sprites = Robot::make_sprites(file_name);

void Robot::rotate(int deg){
    angle = modulo((angle + deg), 360);
    current_image = sprites[angle];
    printf(", robot: %p, angle: %d\n", this, angle);

}

void Robot::move(int distance){
    int delta_x = double(distance) * cos(angle*PI/180);

    //I want to use quadrant 1 angles and quadrant 4 positions
    //Fuck graphics coors
    int delta_y = -1 * double(distance) * sin(angle*PI/180);

    percise_x = fmod((percise_x + delta_x), 1080);
    percise_y = fmod((percise_y+ delta_y), 720);
    
    position.x = (position.x + delta_x)%1080;
    position.y = (position.y + delta_y)%720;

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

void Robot::step(){
    Neuron *left_garden, *right_garden, *clock, *left_turn, *right_turn, *tail_motor;

    //Handle inputs
    left_garden = nnet->get_neuron("left_garden");
    right_garden = nnet->get_neuron("right_garden");
    clock = nnet->get_neuron("clock");
    
    left_garden->self_stimulate(sense("left_garden"));
    right_garden->self_stimulate(sense("right_garden"));
    //Always fire the clock
    clock->self_stimulate(100000);

    //Crank the nnet(I wrote a test for this. I hope it works :) )
    nnet->step();

    //Handle outputs
    left_turn = nnet->get_neuron("left_turn"); 
    right_turn = nnet->get_neuron("right_turn");
    tail_motor = nnet->get_neuron("tail_motor");

    //Turns will cancel each other out
    if(left_turn->get_output() > 0 && right_turn->get_output() == 0){
        printf("angle current: %d\n", angle);
        printf("Turning left\n");
        rotate(10);
        printf("angle after rotate: %d\n", angle);
    }
    
    if(right_turn->get_output() > 0 && left_turn->get_output() == 0){
        printf("Turning right");
        rotate(-10);
    }

    if(tail_motor->get_output() > 0){
        move(5);
    }

}

void Robot::signal_garden(Garden *garden){
    if(last_garden != garden && num_of_fruit < fruit_cap){
        harvest(garden);
    }
}

void Robot::harvest(Garden *garden){
    if(garden->harvest()){
        printf("Should get called2");
        num_of_fruit += 1;
        last_garden = garden;
    }
}

int Robot::get_fruit(){
    return num_of_fruit;
}

int Robot::get_score(){
    return num_of_fruit + money;
}

void Robot::sell_a_fruit(){
    if(num_of_fruit > 0){
        money ++;
        num_of_fruit--;
    }
}

vector<unsigned int> Robot::get_ancestors(){
    return ancestors;
}

void Robot::set_ancestors(std::vector<unsigned int> a){
    ancestors = a;
}

void Robot::reset_score(){
    num_of_fruit = 0;
    money = 0;
}

Robot::~Robot(){
    if(nnet != NULL){
        delete nnet;
    }
}






