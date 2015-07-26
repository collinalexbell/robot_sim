#ifndef AGENT_H
#define AGENT_H

#include "drawable.h"

class Agent: public Drawable{
    public:
        virtual int get_angle() = 0; 
};
#endif
