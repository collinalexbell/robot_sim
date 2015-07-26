#ifndef WORLD_H
#define WORLD_H

class Robot;

#include "garden.h"
#include "robot.h"
#include "drawable.h"
#include "point.h"
#include <unordered_map>
#include <vector>


class World: public Drawable{

    public:
        World();
        unsigned int add_robot(int x, int y);
        unsigned int add_robot(int x, int y, std::string json_text);
        unsigned int add_garden(int x, int y);
        Robot* get_robot(unsigned int);
        Garden* get_garden(unsigned int);
        std::vector<Garden*> get_gardens();
        SDL_Surface* get_image_of_robots();
        Point get_position();
        SDL_Surface* get_image();
        void remove_robot(unsigned int uuid);
        void remove_garden(unsigned int uuid);

    private:
        std::unordered_map<unsigned int, Robot*> robots;
        std::unordered_map<unsigned int, Garden*> gardens;
        std::vector<Garden*> garden_vec;
        SDL_Surface* make_background();
        SDL_Surface* world_surf;
        SDL_Surface* background;
};

#endif
