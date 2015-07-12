#ifndef ROBOT_H
#define ROBOT_H

#include <SDL/SDL.h>
#include "point.h"

struct Radiation{
    //This pointer must always remain valid
    SDL_Surface* image;
    Point location;
};

class Robot{
    public:
        Robot();
        void set_position(int x, int y);
        Point get_position();
        Radiation radiate();

    private:
        Point position;
        bool position_exists;
        SDL_Surface* current_image;
        


};

#endif
