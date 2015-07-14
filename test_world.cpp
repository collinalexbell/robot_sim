#define CATCH_CONFIG_RUNNER
#include "../libs/catch.hpp"
#include "../sim.h"
#include "../world.h"
#include <SDL/SDL.h>

TEST_CASE( "world can add and draw robots" ){
    Simulator* sim = new Simulator();

    World* world = new World();
    world->add_robot(30,30);
    SDL_Surface robot_surf = world->get_image_of_robots();

    sim->add_drawable(robot_surf);

    int test_id = sim->start_gui_test_bool("Is the robot being displayed", true);
    bool gui_test_result = false;

    while(sim->step()){
        if(sim->gui_test_finished(test_id)){
            gui_test_result = sim->get_gui_test_result(test_id);
            break;
        }
    }
    REQUIRE( gui_test_result == result);

}

int main( int argc, char* argv[] )
{
      
    int result = Catch::Session().run( argc, argv );
     
          // global clean-up...
    
    return result;
}
