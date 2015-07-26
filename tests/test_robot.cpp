#define CATCH_CONFIG_RUNNER
#include "../libs/catch.hpp"
#include "robot.h"
#include "spiking_nnet.h"
#include "world.h"
#include "point.h"
#include "sim.h"
#include <SDL/SDL.h>
#include <iostream>
#include <fstream>

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

TEST_CASE("Robot can sense gardens"){
    Robot* robot;
    Garden* garden;
    double garden_left, garden_right;
    World *world = new World();
    int robot_uuid, garden_uuid;

    robot_uuid = world->add_robot(0, 50);
    garden_uuid = world->add_garden(1000, 50);

    robot = world->get_robot(robot_uuid);
    garden = world->get_garden(garden_uuid);

    //Robot can sense garden

    garden_left = robot->sense("left_garden");
    garden_right = robot->sense("right_garden");

    REQUIRE( garden_left == garden_right );
    REQUIRE( garden_left > 0 );

}

TEST_CASE("Robots, when created, initialize a new standard neural net"){

    std::ifstream t("resources/prototype_spikingnn.json");
    std::string json_text((std::istreambuf_iterator<char>(t)),
                             std::istreambuf_iterator<char>());

    Robot* robot;
    Spiking_NNet* nnet;
    vector<std::string> neurons_needed;
    Neuron* n;

    robot = new Robot(json_text);
    nnet = robot->get_nnet();
    
    neurons_needed = {"right_garden", 
                      "left_garden", 
                      "left_customer",
                      "right_customer",
                      "left_turn",
                      "right_turn"};

    for( auto it = neurons_needed.begin(); it != neurons_needed.end(); it++ ){
        n = nnet->get_neuron((*it));
        REQUIRE(typeid(*n) == typeid(Neuron));
    }

}

TEST_CASE("Robots, have a step function that senses gardens, runs nnet, and acts based on the nnet"){
    //WILL NEED TO CHANGE IF HARDCODED angle and distance variables in
    //robot->step change
    Robot *robot;
    World *world;
    Spiking_NNet *nnet;
    Neuron *left_garden, *right_garden, *left_turn, *right_turn, *tail_motor, *clock;
    int garden_uuid, robot_uuid;

    std::ifstream t("resources/prototype_spikingnn.json");
    std::string json_text((std::istreambuf_iterator<char>(t)),
                             std::istreambuf_iterator<char>());


    world = new World();
    robot_uuid = world->add_robot(0,100, json_text);
    world->add_garden(0, 100);
    robot = world->get_robot(robot_uuid);

    //The garden should trigger both the left_garden and right_garden sensor
    robot->step();

    int entry = 0;

    nnet = robot->get_nnet();
    left_garden = nnet->get_neuron("left_garden");
    entry ++;
    right_garden = nnet->get_neuron("right_garden");
    entry ++;
    clock = nnet->get_neuron("clock");
    entry ++;

    REQUIRE(left_garden->get_output() > 0);
    REQUIRE(right_garden->get_output() > 0);
    REQUIRE(clock->get_output() > 0);

    
    //Now make robot rotate left
    
    //Need to remove the garden and remake robot
    //This will prevent garden stim from messing up movement
    //Also, clock needs its threshold set to 10000000ish for same reason
    
    world->remove_robot(robot_uuid);
    world->remove_garden(garden_uuid);

    robot_uuid = world->add_robot(0,100, json_text);
    world->add_garden(0, 100);
    robot = world->get_robot(robot_uuid);

    nnet = robot->get_nnet();
    clock = nnet->get_neuron("clock");
    //EAT THIS CLOCK!!!!!
    clock->set_threshold(100000000000);

    left_turn = nnet->get_neuron("left_turn");
    entry ++;
    left_turn->self_stimulate(1.1);

    robot->step();

    REQUIRE(robot->get_angle() == 3);

    right_turn = nnet->get_neuron("right_turn");
    entry ++;
    right_turn->self_stimulate(1.1);

    robot->step();

    REQUIRE(robot->get_angle() == 0);

    //Now make robot move!!

    tail_motor = nnet->get_neuron("tail_motor");
    tail_motor->self_stimulate(1.1);
    
    robot->step();

    Point pos = robot->get_position();
    REQUIRE(pos.x == 5);
    
}

TEST_CASE("robot can handle garden and customer signal"){
    Robot *robot;
    Garden *garden1, *garden2;
    robot = new Robot();
    garden1 = new Garden();
    garden2 = new Garden();


    //Default garden cap is 3 for robot
    //Default fruit gardens produce is 3;
    
    robot->signal_garden(garden1);
    REQUIRE(garden1->get_remaining_fruit() == 2);
    REQUIRE(robot->get_fruit() == 1);

    //Gardens should not be able to be harvest twice
    robot->signal_garden(garden1);
    REQUIRE(garden1->get_remaining_fruit() == 2);
    REQUIRE(robot->get_fruit() == 1);

    //Go to another garden
    robot->signal_garden(garden2);
    REQUIRE(garden2->get_remaining_fruit() == 2);
    REQUIRE(robot->get_fruit() == 2);

    //Go back to original
    robot->signal_garden(garden1);
    REQUIRE(garden1->get_remaining_fruit() == 1);
    REQUIRE(robot->get_fruit() == 3);

    //Go to another garden. Robot cant harvest, too full
    robot->signal_garden(garden2);
    REQUIRE(garden2->get_remaining_fruit() == 2);
    REQUIRE(robot->get_fruit() == 3);

    robot->sell_a_fruit();

    //This should work because it was blocked last time
    robot->signal_garden(garden2);
    REQUIRE(garden2->get_remaining_fruit() == 1);
    REQUIRE(robot->get_fruit() == 3);





}



int main( int argc, char* argv[] )
{
      
    int result = Catch::Session().run( argc, argv );
     
          // global clean-up...
    
    return result;
}
