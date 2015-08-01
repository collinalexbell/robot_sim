#ifndef ROBOT_H
#define ROBOT_H

class World;

#include <SDL/SDL.h>
#include "point.h"
#include "drawable.h"
#include "agent.h"
#include "world.h"
#include "distance_sensor.h"
#include "spiking_nnet.h"
#include <vector>
#include <unordered_map>
#include <string>

using namespace std;

class Robot: public Agent{
    private:
        World* world;
    public:
        Robot();
        ~Robot();
        Robot(std::string);

        void add_world(World* w){
            world = w;
        }

        //Used when initializing robot
        void set_position(int x, int y);

        //Makes 360 sprites. 1 per degree of rotation
        static vector<SDL_Surface*> make_sprites(const char* file);

        //Abstract Drawable Implementation
        SDL_Surface* get_image();

        //Abstract Drawable Implementation
        Point get_position();

        //Obvious, int is amout to rotate in deg
        void rotate(int);

        int get_angle(){
            return angle;
        }

        //Obvous, int is abount to move in distance 
        //NOTE: x, y components will be rounded
        void move(int);

        void add_distance_sensor(std::string name, double angle, double width);

        double sense(std::string sensor_name);

        Spiking_NNet* get_nnet();

        void step();

        //Common to constructors
        void init();

        void signal_garden(Garden* garden);

        int get_fruit();

        void harvest(Garden*);

        void sell_a_fruit();

        int get_money();

        int get_score();

        void give_fruit();

        std::vector<unsigned int>get_ancestors();

        void set_ancestors(vector<unsigned int>);

        void reset_score();

        double percise_x, percise_y;

    private:
        //Internal direction
        int angle;

        //360 deg of sprites
        static vector<SDL_Surface*> sprites;

        //Obvious
        Point position;

        //Flag to posibly complain on get_position()
        bool position_exists;

        //Obvious. Is set on movements and rotates
        SDL_Surface* current_image;

        //Used to store garden and  customer sensors
        std::unordered_map<std::string, Distance_Sensor*> distance_sensors;

        Spiking_NNet* nnet; 
    
        //Keeps track of last garden harvested so bot doesn't double harvest
        Garden *last_garden;
        
        int num_of_fruit;
        
        //Max fruit robot can cary: Hardcoded to 3
        int fruit_cap;

        //Earned from selling fruit
        int money;
        
        std::vector<unsigned int> ancestors;

};

#endif
