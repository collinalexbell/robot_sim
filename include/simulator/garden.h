#ifndef GARDEN_H
#define GARDEN_H

#include "point.h"
#include "drawable.h"
#include <SDL/SDL.h>

class Garden: public Drawable{
    public:
        Garden();

        //Abstract Drawable Implementation
        SDL_Surface* get_image();

        //Abstract Drawable Implementation
        Point get_position();

        void set_position(int x, int y);

        bool harvest();

        int get_remaining_fruit();

    private:
        SDL_Surface* garden_sprite;
        Point position;
        bool position_exists;
        int fruit_remaining;
};

#endif
