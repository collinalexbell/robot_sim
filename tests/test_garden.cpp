#define CATCH_CONFIG_RUNNER
#include "../libs/catch.hpp"
#include "../sim.h"
#include <SDL/SDL.h>


int main( int argc, char* argv[] )
{
      
    int result = Catch::Session().run( argc, argv );
     
          // global clean-up...
    
    return result;
}

