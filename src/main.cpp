#include <stdio.h>
#include "sim.h"
#include <string>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

void cb(void *s){
    Sim *sim;
    sim = static_cast<Sim*>(s);
    sim->step();
}

extern "C" int main(int argc, char* argv[]) {
    //Robot* robot = new Robot();
    //robot->set_position(20,20);

    Sim *sim = new Sim(1080, 780);
    void *em_sim;
    em_sim = &sim;
    printf("Printing debug info");
    sim->init();
    //sim->add_drawable(robot);

//    sim->make_world(0,0);


    emscripten_set_main_loop_arg(cb, em_sim, 0, false);
    


    return 1;
}
