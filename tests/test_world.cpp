#define CATCH_CONFIG_RUNNER
#include "../libs/catch.hpp"
#include "sim.h"
#include "world.h"
#include <SDL/SDL.h>

void gui_test(Sim* sim, char *text, bool result){
    unsigned int test_id = sim->start_gui_test_bool(text, result);
    bool gui_test_result = false;

    while(sim->step()){
        if(sim->gui_test_finished(test_id)){
            gui_test_result = sim->get_gui_test_result(test_id);
            break;
        }
    }
    REQUIRE( gui_test_result == result);
}

TEST_CASE( "world can draw" ){
    Sim* sim = new Sim(1080, 760);
    sim->init();

    World* world = new World();

    sim->add_drawable(world);

    gui_test(sim, "World is drawn with background", true);
    
}

TEST_CASE( "world can add and draw robots" ){
    Sim* sim = new Sim(1080, 760);
    sim->init();

    World* world = new World();
    world->add_robot(100,100);

    sim->add_drawable(world);

    gui_test(sim, "Robot is drawn", true);

}

TEST_CASE( "world can add and draw gardens" ){
    Sim* sim = new Sim(1080, 760);
    sim->init();

    World* world = new World();
    world->add_garden(100,100);

    sim->add_drawable(world);

    gui_test(sim, "Garden is drawn", true);

}


int main( int argc, char* argv[] )
{
      
    int result = Catch::Session().run( argc, argv );
     
          // global clean-up...
    
    return result;
}
