#include <stdio.h>
#include "sim.h"
#include <string>

extern "C" int main(int argc, char* argv[]) {
    Sim* sim = new Sim(1080, 780);
    sim->init();

    sim->make_world(100,3);

    while(sim->step()){
    }


    return 1;
}
