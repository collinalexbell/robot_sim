#ifndef ROBOT_H
#define ROBOT_H

#include <SDL/SDL.h>
#include "point.h"
#include "drawable.h"
#include <vector>

using namespace std;

class Robot: public Drawable{
    public:
        Robot();

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

        //Obvous, int is abount to move in distance 
        //NOTE: x, y components will be rounded
        void move(int);


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
        


};

#endif
