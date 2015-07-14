#define CATCH_CONFIG_RUNNER
#include "../libs/catch.hpp"
#include "../robot.h"
#include "../point.h"
#include <SDL/SDL.h>
#include <iostream>

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
    Radiation radiation = robot->radiate();
    REQUIRE(radiation.location.x == 5);
    REQUIRE(radiation.location.y == 7);
    REQUIRE(typeid(*radiation.image) == typeid(SDL_Surface));

}

int main( int argc, char* argv[] )
{
      
    int result = Catch::Session().run( argc, argv );
     
          // global clean-up...
    
    return result;
}
