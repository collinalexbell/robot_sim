#define CATCH_CONFIG_RUNNER
#include "../libs/catch.hpp"
#include "garden.h"
#include <SDL/SDL.h>

TEST_CASE("Garden can get position and image"){
    // This case makes a garden without any parameters. 
    // The only param that should ever be passed to a garden is a Zygote.
    Garden* garden = new Garden();

    //Radiation has a Point position and SDL_Surface image


    //Normal usage 
    garden->set_position(5,7);
    Point point = garden->get_position();
    REQUIRE(point.x == 5);
    REQUIRE(point.y == 7);
    REQUIRE(typeid(*garden->get_image()) == typeid(SDL_Surface));

}

int main( int argc, char* argv[] )
{
      
    int result = Catch::Session().run( argc, argv );
     
          // global clean-up...
    
    return result;
}

