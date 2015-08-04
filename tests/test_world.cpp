#define CATCH_CONFIG_RUNNER
#include "../libs/catch.hpp"
#include "sim.h"
#include "world.h"
#include <SDL/SDL.h>

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
    for( int i=0; i<100; i++){
        world->add_robot(i,i);
    }

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

TEST_CASE( "world will signal robot when it runs over a garden" ){
    World *world;
    Robot *robot;
    Garden *garden;
    int robot_uuid, garden_uuid;
    world = new World();

    robot_uuid = world->add_robot(0,100);
    garden_uuid = world->add_garden(100, 100);

    robot = world->get_robot(robot_uuid);
    garden = world->get_garden(garden_uuid);



    world->step();

    REQUIRE(robot->get_fruit() == 0);

    robot->move(104);

    world->step();

    REQUIRE(robot->get_fruit() == 1);


}

TEST_CASE( "world will remove garden once it has been harvested 3 times" ){
    World *world;
    Robot *robot1, *robot2, *robot3;
    Garden *garden;
    Sim *sim;
    int robot_uuid1, robot_uuid2, robot_uuid3, garden_uuid;

    world = new World();

    sim = new Sim(1080, 780);
    sim->init();
    sim->add_drawable(world);

    robot_uuid1 = world->add_robot(0, 100);
    robot_uuid2 = world->add_robot(0, 100);
    robot_uuid3 = world->add_robot(0, 100);
    garden_uuid = world->add_garden(400, 100);

    robot1 = world->get_robot(robot_uuid1);
    robot2 = world->get_robot(robot_uuid2);
    robot3 = world->get_robot(robot_uuid3);
    garden = world->get_garden(garden_uuid);

    
    gui_test(sim, "Garden disappears and appears in a new spot after 3 harvests and then regrows elsewhere", true, [&](){
        if(robot1->get_fruit() == 0 ){
            robot1->move(40);
            world->step();
        }
        else if(robot2->get_fruit() == 0 ){
            robot2->move(40);
            world->step();
        }
        else if(robot3->get_fruit() == 0 ){
            robot3->move(40);
            world->step();
        }
    });

}

TEST_CASE("Mutate World"){
    //Sim* sim;
    World* world;
    int i, num_bots_without_ancestors, num_bots_with_ancestors;
    std::vector<Robot*> robots;
    std::vector<Robot*> victors;

    world = new World();
    //Add 10 robots
    for( i=0; i<10; i++ ){
        world->add_robot(0,0);
    }

    robots = world->get_robots();
    robots[0]->give_fruit();
    REQUIRE(robots[0]->get_fruit() == 1);
    //30% of bots get to mutate
    world->mutate(.3);
    //More robots were added and some where changed
    robots = world->get_robots();

    num_bots_without_ancestors = 0;
    num_bots_with_ancestors = 0;
    for( auto it=robots.begin(); it != robots.end(); it++ ){
        if((*it)->get_ancestors().size() == 0){
            num_bots_without_ancestors++;
            victors.push_back(*it);
        }else{
            num_bots_with_ancestors++;
        }
    }

    REQUIRE(num_bots_without_ancestors == 3);
    REQUIRE(num_bots_with_ancestors == 7);
    //Mutate world must reset score!
    for( auto it=victors.begin(); it!=victors.end(); it++ ){
        REQUIRE((*it)->get_score() == 0);
    }

}

TEST_CASE("World can get_drawables"){
    int i;
    Sim* sim = new Sim(1080, 760);
    sim->init();
    World* world = new World();
    for(i=0;i<5;i++){
        world->add_robot(100, i*20);
    }

    sim->add_drawables(world->get_drawables());

    gui_test(sim, "Robots get drawn", true);


}



int main( int argc, char* argv[] )
{
      
    int result = Catch::Session().run( argc, argv );
     
          // global clean-up...
    
    return result;
}
