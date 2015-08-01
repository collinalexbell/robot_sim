#define CATCH_CONFIG_RUNNER
#include "../libs/catch.hpp"
#include "sim.h"
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <iostream>

using namespace std;

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

TEST_CASE( "sim get_screen returns a screen", "[screen]" ) {
    Sim* sim = new Sim(1080, 780);
    sim->init(); 
    REQUIRE(typeid(*(sim->get_screen())) == typeid(SDL_Surface));
}

TEST_CASE( "sim gui_test works", "[gui_test]" ){
    Sim* sim = new Sim(1080, 780);
    sim->init();
    gui_test(sim, "Gui test works", true);
    gui_test(sim, "Gui test is broken", false);

}

TEST_CASE( "sim add_drawable"){
    Sim *sim = new Sim(1080, 780);
    sim->init();

    SDL_Surface* torus_surf = IMG_Load("tests/torus.png");
    Point p = {300, 50};
    Basic_Drawable* torus = new Basic_Drawable(torus_surf, p);
    std::cout << "error " << SDL_GetError() << std::endl;
    
    sim->add_drawable(torus);

    gui_test(sim, "Sim loads and draws torus", true);
}

TEST_CASE( "Sim can run a full world simulation" ){
    Sim *sim = new Sim(1080, 780);
    sim->init();

    //Make new world with 50 robots and 3 gardens
    sim->make_world(100, 3);
    
    gui_test(sim, "Sim makes a new world of 10 robots and 3 gardens and runs the sim", true);
}


int main( int argc, char* argv[] )
{
      
    int result = Catch::Session().run( argc, argv );
     
          // global clean-up...
    
    return result;
}
