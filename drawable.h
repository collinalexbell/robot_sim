#ifndef DRAWABLE_H
#define DRAWABLE_H

#include "point.h"

class Drawable{
    public:
        virtual SDL_Surface* get_image() = 0;
        virtual Point get_position() = 0;
};

class Basic_Drawable: public Drawable{
    public:
        Basic_Drawable(SDL_Surface* im, Point p ){
            image = im;
            point = p;
        }
        SDL_Surface* get_image(){
            return image;
        }
        Point get_position(){
            return point;
        }
    private:
        SDL_Surface* image;
        Point point;
};

#endif
