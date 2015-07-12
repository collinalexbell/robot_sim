#include <stdio.h>
#include "sim.h"

extern "C" int main(int argc, char* argv[]) {
    Sim* sim = new Sim(1080, 720);
    sim->init();
    int test_id = sim->start_gui_test_bool("Gui test works", true);
    bool gui_test_result = false;

    while(sim->step()){
        if(sim->gui_test_finished(test_id)){
            bool gui_test_result = sim->get_gui_test_result(test_id);
        }
    }
}
