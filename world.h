#ifndef WORLD_H
#define WORLD_H

#include "robot.h"
#include "drawable.h"
#include "point.h"
#include <unordered_map>


class World: public Drawable{

    public:
        World();
        unsigned int add_robot(int x, int y);
        SDL_Surface* get_image_of_robots();
        Point get_position();
        SDL_Surface* get_image();

    private:
        std::unordered_map<unsigned int, Robot*> robots;
        SDL_Surface* make_background();
        SDL_Surface* world_surf;
        SDL_Surface* background;
};

#endif
