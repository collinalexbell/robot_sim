#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "garden.h"


Garden::Garden(){
    position_exists = false; 

    //Load Image
    garden_sprite = IMG_Load("./resources/blue_garden.png");

    fruit_remaining = 3;


}

void Garden::set_position(int x, int y){
    position.x = x;
    position.y = y;
    position_exists = true;
}

Point Garden::get_position(){
    if(position_exists){
        return position;
    }
    else{
        throw "Position does not exist";
    }
}

SDL_Surface* Garden::get_image(){
    return garden_sprite;
}

bool Garden::harvest(){
    if( fruit_remaining > 0 ){
        fruit_remaining --;
        return true;
    }
    return false;
}

int Garden::get_remaining_fruit(){
    return fruit_remaining;
}
