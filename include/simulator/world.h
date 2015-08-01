#ifndef WORLD_H
#define WORLD_H

class Robot;

#include "garden.h"
#include "robot.h"
#include "drawable.h"
#include "point.h"
#include <unordered_map>
#include <vector>
#include <string>


class World: public Drawable{

    public:
        World();
        unsigned int add_robot(int x, int y);
        unsigned int add_robot(int x, int y, std::string json_text);
        unsigned int add_robot(int x, int y, std::string json_text, std::vector<unsigned int> ancestors);
        unsigned int add_garden(int x, int y);
        Robot* get_robot(unsigned int);
        Garden* get_garden(unsigned int);
        std::vector<Garden*> get_gardens();
        std::vector<Robot*> get_robots();
        SDL_Surface* get_image_of_robots();
        Point get_position();
        SDL_Surface* get_image();
        void remove_robot(unsigned int uuid);
        void remove_garden(unsigned int uuid);
        void step();
        void mutate(double ratio_to_reproduce);
        void handle_robot_garden_collisions();
        bool check_collision(SDL_Rect, SDL_Rect);

    private:
        int last_mutate_time;
        int mutate_time;
        std::unordered_map<unsigned int, Robot*> robots;
        std::unordered_map<Robot*, unsigned int> robot_uuid_lookup;
        std::unordered_map<unsigned int, Garden*> gardens;
        std::unordered_map<Garden*, unsigned int> garden_uuid_lookup;
        std::vector<Garden*> garden_vec;
        std::vector<Robot*> robot_vec;
        SDL_Surface* make_background();
        SDL_Surface* world_surf;
        SDL_Surface* background;
        int height, width;
};

#endif
