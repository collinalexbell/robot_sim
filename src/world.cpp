#include "world.h"
#include "robot.h"
#include "garden.h"
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <stdlib.h>
#include <iostream>

#define MAX_ID 65535

World::World(){
    world_surf = SDL_CreateRGBSurface(0,1080, 720, 32,0,0,0,0);
    background = make_background();
}


unsigned int World::add_robot(int x, int y){
    unsigned int uuid = rand() % MAX_ID;
    Robot* r = new Robot();
    r->set_position(x,y);
    std::pair<unsigned int, Robot*> insert_me (uuid, r);
    robots.insert(insert_me);

    return uuid;
}

unsigned int World::add_garden(int x, int y){
    unsigned int uuid = rand() % MAX_ID;
    Garden* g = new Garden();
    g->set_position(x,y);
    std::pair<unsigned int, Garden*> insert_me (uuid, g);
    gardens.insert(insert_me);

    return uuid;
}

Robot* get_robot(unsigned int id){
    return robots.at(id);
}

Garden* get_garden(unsigned int id){
    return gardens.at(id);
}

SDL_Surface* World::get_image(){
    SDL_Rect* offset = new SDL_Rect();

    //Blit background
    SDL_BlitSurface(background, NULL, world_surf, NULL);
    
    //Blit robots
    for ( auto it = robots.begin(); it != robots.end(); ++it ){
        Point point = it->second->get_position();

        offset->x = point.x;
        offset->y = point.y;

        SDL_BlitSurface(it->second->get_image(), NULL, world_surf, offset);
    }

    //Blit robots
    for ( auto it = gardens.begin(); it != gardens.end(); ++it ){
        Point point = it->second->get_position();

        offset->x = point.x;
        offset->y = point.y;

        SDL_BlitSurface(it->second->get_image(), NULL, world_surf, offset);
    }

    //Clean up
    delete offset;

    
    return world_surf;
}

Point World::get_position(){
    Point rv = {0,0};
    return rv;
}

SDL_Surface* World::make_background(){
    SDL_Rect* offset = new SDL_Rect();
    int i, j, x_blits, y_blits;
    SDL_Surface* floor_tile = IMG_Load("resources/floor.jpg");

    x_blits = world_surf->w % floor_tile->w;
    y_blits = world_surf->h % floor_tile->h;

    for( i=0; i<x_blits; i++ ){
        for ( j=0; j<y_blits; j++ ){
           offset->x = i * floor_tile->w; 
           offset->y = j * floor_tile->h;

           SDL_BlitSurface(floor_tile, NULL, world_surf, offset);
        }
    }

    delete offset;
    offset = NULL;
}
