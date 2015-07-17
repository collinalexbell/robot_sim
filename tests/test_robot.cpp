#define CATCH_CONFIG_RUNNER
#include "../libs/catch.hpp"
#include "../robot.h"
#include "../world.h"
#include "../point.h"
#include "../sim.h"
#include <SDL/SDL.h>
#include <iostream>

template<typename Func>
void gui_test(Sim* sim, char text[], bool result, Func func =[](){}){
    unsigned int test_id = sim->start_gui_test_bool(text, result);
    bool gui_test_result = false;

    while(sim->step()){
        if(sim->gui_test_finished(test_id)){
            gui_test_result = sim->get_gui_test_result(test_id);
            break;
        }
        func();
    }
    REQUIRE( gui_test_result == result);
}


TEST_CASE("Robot can set and return its position"){
    Robot* robot = new Robot();
    Point position;

    //get_position without a robot having a position should throw an exception
    REQUIRE_THROWS(position = robot->get_position());


    robot->set_position(5,8);
    position = robot->get_position();

    REQUIRE(position.x == 5);
    REQUIRE(position.y == 8);

}

TEST_CASE("Robot can radiate light"){
    // This case makes a robot without any parameters. 
    // The only param that should ever be passed to a robot is a Zygote.
    Robot* robot = new Robot();

    //Radiation has a Point position and SDL_Surface image


    //Normal usage 
    robot->set_position(5,7);
    Point point = robot->get_position();
    REQUIRE(point.x == 5);
    REQUIRE(point.y == 7);
    REQUIRE(typeid(*robot->get_image()) == typeid(SDL_Surface));

}

TEST_CASE("Robot has 360 sprites, accessable by rotate"){
    Robot* robot = new Robot();
    robot->set_position(20,20);

    Sim* sim = new Sim(1080,780);
    sim->init();
    sim->add_drawable(robot);

    gui_test(sim, "tests that the robot rotates", true, [&](){
        robot->rotate(1);
    });

    gui_test(sim, "Can the robot can rotate many revolution",  true, [&](){
        robot->rotate(20);
    });

    gui_test(sim, "Can the robot rotate negatively", true, [&](){
            robot->rotate(-1);
    });
}

TEST_CASE("Robot can move at various speeds"){
    Robot* robot = new Robot();
    robot->set_position(20,20);

    Sim* sim = new Sim(1080, 780);
    sim->init();
    sim->add_drawable(robot);

    gui_test(sim, "Does robot move slow", true, [&](){
            robot->move(1);
    });


    robot->set_position(20, 20);
    robot->rotate(-30);
    gui_test(sim, "Does robot move faster", true, [&](){
            robot->move(7);
    });

    robot->set_position(20, 20);
    robot->rotate(-60);
    gui_test(sim, "Does robot move really fast", true, [&](){
            robot->move(20);
    });
}



int main( int argc, char* argv[] )
{
      
    int result = Catch::Session().run( argc, argv );
     
          // global clean-up...
    
    return result;
}
