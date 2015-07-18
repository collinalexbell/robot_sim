#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_rotozoom.h>
#include "robot.h"
#include "point.h"
#include <iostream>
#include <math.h>

#define PI 3.14159265

inline int modulo(int a, int b) {
      const int result = a % b;
        return result >= 0 ? result : result + b;
}

Robot::Robot(){
   position_exists = false; 
   current_image = IMG_Load("./resources/ball_bot.png");
   std::cout << SDL_GetError() << std::endl;
   angle = 0;
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
