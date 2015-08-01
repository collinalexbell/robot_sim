#include "world.h"
#include "robot.h"
#include "garden.h"
#include "distance_sensor.h"
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <string>
#include <fstream>
#include <time.h>

#define MAX_ID 65535

World::World(){
    srand (time(NULL));
    width = 1080;
    height = 720;
    world_surf = SDL_CreateRGBSurface(0,width, height, 32,0,0,0,0);
    background = make_background();
    last_mutate_time = time(NULL);
    mutate_time = 60;
}


unsigned int World::add_robot(int x, int y){
    std::ifstream t("resources/prototype_spikingnn.json");
        std::string json_text((std::istreambuf_iterator<char>(t)),
            std::istreambuf_iterator<char>());



    printf("Made it into add_robot");
    unsigned int uuid = rand() % MAX_ID;
    printf("Made new uuid first %d\n", uuid);
    Robot* r = new Robot(json_text);
    r->add_world(this);
    r->set_position(x,y);
    r->add_distance_sensor("left_garden", 20, 60);
    r->add_distance_sensor("right_garden", -20, 60);
    r->add_distance_sensor("left_customer", 20, 60);
    r->add_distance_sensor("right_customer", -20, 60);
    std::pair<unsigned int, Robot*> insert_me (uuid, r);
    robots.insert(insert_me);
    std::pair<Robot*, unsigned int> insert_me2 (r, uuid);
    robot_uuid_lookup.insert(insert_me2);
    robot_vec.push_back(r);

    return uuid;
}

unsigned int World::add_robot(int x, int y, std::string json_text){
    unsigned int uuid = rand() % MAX_ID;
    printf("Made new uuid second %d\n", uuid);
    Robot* r = new Robot(json_text);
    r->add_world(this);
    r->set_position(x,y);
    r->add_distance_sensor("left_garden", 20, 60);
    r->add_distance_sensor("right_garden", -20, 60);
    r->add_distance_sensor("left_customer", 20, 60);
    r->add_distance_sensor("right_customer", -20, 60);
    std::pair<unsigned int, Robot*> insert_me (uuid, r);
    robots.insert(insert_me);
    std::pair<Robot*, unsigned int> insert_me2 (r, uuid);
    robot_uuid_lookup.insert(insert_me2);
    robot_vec.push_back(r);

    return uuid;
}

unsigned int World::add_robot(int x, int y, std::string json_text, std::vector<unsigned int> ancestors){
    unsigned int uuid;
    Robot *robot;

    uuid = add_robot(x, y, json_text);
    robot = get_robot(uuid);
    robot->set_ancestors(ancestors);
}

unsigned int World::add_garden(int x, int y){
    unsigned int uuid = rand() % MAX_ID;
    Garden* g = new Garden();
    g->set_position(x,y);
    std::pair<unsigned int, Garden*> insert_me (uuid, g);
    std::pair<Garden*, unsigned int> insert_me2 (g, uuid);
    gardens.insert(insert_me);
    garden_uuid_lookup.insert(insert_me2);
    garden_vec.push_back(g);

    return uuid;
}

void World::remove_robot(unsigned int uuid){
    try{
        Robot* robot = robots.at(uuid);
        auto it = find(robot_vec.begin(), robot_vec.end(), robot);
        robot_vec.erase(it);
        robot_uuid_lookup.erase(robot);
        delete robot;
        robots.erase(uuid);
    }
    catch(...){
        printf("Failed to get robot at uuid: %d", uuid);
    }
}

void World::remove_garden(unsigned int uuid){
    try{
        Garden* garden = gardens.at(uuid);
        auto it = find(garden_vec.begin(), garden_vec.end(), garden);
        gardens.erase(uuid);
        garden_vec.erase(it);
        garden_uuid_lookup.erase(garden);
        delete garden;
    }
    catch(...){
        printf("Failed to get garden at uuid: %d", uuid);
    }


}

Robot* World::get_robot(unsigned int id){
    try{
        return robots.at(id);
    }catch(...){
        char buffer[100];
        sprintf(buffer, "World doesn't have a robot with id %d", id);
        throw buffer;
    }
}

Garden* World::get_garden(unsigned int id){
    return gardens.at(id);
}

SDL_Surface* World::get_image(){
    SDL_FillRect(world_surf, NULL, 0x000000);
    SDL_Rect* offset = new SDL_Rect();

    //Blit background
    SDL_BlitSurface(background, NULL, world_surf, offset);
    
    //Blit robots
    for ( auto it = robots.begin(); it != robots.end(); ++it ){
        Point point = it->second->get_position();

        offset->x = point.x;
        offset->y = point.y;

        SDL_BlitSurface(it->second->get_image(), NULL, world_surf, offset);
    }

    //Blit robots
    for ( auto it = gardens.begin(); it != gardens.end(); ++it ){
        Point point = it->second->get_position();

        offset->x = point.x;
        offset->y = point.y;

        SDL_BlitSurface(it->second->get_image(), NULL, world_surf, offset);
    }

    //Clean up
    delete offset;

    
    return world_surf;
}

Point World::get_position(){
    Point rv = {0,0};
    return rv;
}

SDL_Surface* World::make_background(){
    SDL_Surface *bk_surf; 
    bk_surf = SDL_CreateRGBSurface(0,1080, 720, 32,0,0,0,0);
    SDL_Rect* offset = new SDL_Rect();
    int i, j, x_blits, y_blits;
    SDL_Surface* floor_tile = IMG_Load("resources/floor.jpg");

    x_blits = world_surf->w % floor_tile->w;
    y_blits = world_surf->h % floor_tile->h;

    for( i=0; i<x_blits; i++ ){
        for ( j=0; j<y_blits; j++ ){
           offset->x = i * floor_tile->w; 
           offset->y = j * floor_tile->h;

           SDL_BlitSurface(floor_tile, NULL, bk_surf, offset);
        }
    }

    delete offset;
    offset = NULL;
    return bk_surf;
}

vector<Garden*> World::get_gardens(){
    return garden_vec;
}

vector<Robot*> World::get_robots(){
    return robot_vec;
}

void World::handle_robot_garden_collisions(){
    int garden_h, garden_w, robot_h, robot_w;
    SDL_Surface *garden_img, *robot_img;
    SDL_Rect garden_rect, robot_rect;
    Point garden_pos, robot_pos;
    std::vector<unsigned int> gardens_to_remove;

    for( auto robot_it = robot_vec.begin(); robot_it != robot_vec.end(); robot_it++ ){
        for( auto garden_it = garden_vec.begin(); garden_it != garden_vec.end(); garden_it++ ){
            garden_img  = (*garden_it)->get_image(); 
            garden_pos = (*garden_it)->get_position();
            
            //Make a rect
            garden_rect.w = garden_img->w;
            garden_rect.h = garden_img->h;
            garden_rect.x = garden_pos.x;
            garden_rect.y = garden_pos.y;

            robot_img  = (*robot_it)->get_image(); 
            robot_pos = (*robot_it)->get_position();

            //Make a rect
            robot_rect.w = robot_img->w;
            robot_rect.h = robot_img->h;
            robot_rect.x = robot_pos.x;
            robot_rect.y = robot_pos.y;

            //Collide rect and act accordingly
            if( check_collision( robot_rect, garden_rect ) ){
                (*robot_it)->signal_garden(*garden_it);
                if( (*garden_it)->get_remaining_fruit() == 0 ){
                    int garden_uuid;
                    for( auto rm_it = gardens.begin(); rm_it != gardens.end(); rm_it++){
                        if(rm_it->second == (*garden_it)){
                            garden_uuid = garden_uuid_lookup[rm_it->second];
                            if(find(gardens_to_remove.begin(), gardens_to_remove.end(), garden_uuid) == gardens_to_remove.end()){
                                gardens_to_remove.push_back(garden_uuid);
                            }
                        }
                    }
                    printf("Removing garden #%d", garden_uuid);
                }
            }
        }
    }
    for( auto it = gardens_to_remove.begin(); it != gardens_to_remove.end(); it++ ){
        remove_garden((*it));
        add_garden(rand()%width, rand()%height);
    }
}


bool World::check_collision( SDL_Rect A, SDL_Rect B ) { 
    //The sides of the rectangles 
    int leftA, leftB; 
    int rightA, rightB;
    int topA, topB; 
    int bottomA, bottomB; 

    //Calculate the sides of rect A 
    leftA = A.x; 
    rightA = A.x + A.w;
    topA = A.y;
    bottomA = A.y + A.h; 

    //Calculate the sides of rect B 
    leftB = B.x; 
    rightB = B.x + B.w; 
    topB = B.y; 
    bottomB = B.y + B.h;

    //If any of the sides from A are outside of B 
    if( bottomA <= topB ) { return false; } 
    if( topA >= bottomB ) { return false; } 
    if( rightA <= leftB ) { return false; } 
    if( leftA >= rightB ) { return false; } 

    //If none of the sides from A are outside B 
    return true; }

void World::mutate(double ratio_to_reproduce){

    std::vector<std::string> top_brains;
    std::string robot_brain;
    std::vector<std::vector<unsigned int> > top_ancestors;
    std::vector<unsigned int> tmp_ancestors;
    std::vector<unsigned int> robot_uuids_to_remove;
    int tmp_score, num_to_keep, num_to_birth, i, rand_robot_index;
    unsigned int uuid;

    num_to_keep = robots.size() * ratio_to_reproduce;
    num_to_birth = robots.size() - num_to_keep;
    std::vector<Robot*> top_bots(num_to_keep, NULL);
    std::vector<int> top_scores (num_to_keep, -1);

    printf("top_bots.size(): %d", top_bots.size());

    //Get robots that need to reproduce
    for( auto it = robot_vec.begin(); it != robot_vec.end(); it++ ){
        tmp_score = (*it)->get_score();
        for( int i=0; i<num_to_keep; i++ ){
            printf("tmp_score: %d\n", tmp_score);
            printf("top_score: %d\n", top_scores[i]);
            if( tmp_score > top_scores[i] ){
                printf("top bot getting changed\n");
                top_bots[i] = (*it);
                top_scores[i] = tmp_score;
                //Don't want the top robot to dominate!!!!
                break;
            }
        }
    }

    //Find and remove robots that didn't get to reproduce
    for( auto it = robot_vec.begin(); it != robot_vec.end(); it++ ){
        //If the robot didn't make the cut
        if(find(top_bots.begin(), top_bots.end(), *it) == top_bots.end()){
            //Get its uuid
            printf("Deleting robot pointer: %p\n", *it);
            uuid = robot_uuid_lookup[*it];
            printf("Deleting robot uuid: %d\n\n", uuid);

            robot_uuids_to_remove.push_back(uuid);
        }
    }

    for( auto it = robot_uuids_to_remove.begin(); it != robot_uuids_to_remove.end(); it++ ){
        remove_robot(*it);
    }

    printf("Num bots remaining: %d", robot_vec.size());

    //Reset scores of top bots
    for( auto it = top_bots.begin(); it!=top_bots.end(); it++ ){
        (*it)->reset_score();
    }

    //Serialize the victor brains
    for( i=0; i<top_bots.size(); i++ ){
        top_brains.push_back(top_bots[i]->get_nnet()->serialize());
    }

    printf("top_brains.size(): %d", top_brains.size());
    //Get the ancestors
    for( i=0; i<top_bots.size(); i++ ){
        tmp_ancestors = top_bots[i]->get_ancestors();
        tmp_ancestors.push_back(robot_uuid_lookup[top_bots[i]]);
        top_ancestors.push_back(tmp_ancestors);
    }

    


    
    //Reproduce the victors
    for( i=0; i<num_to_birth; i++){
        //Select random robot;
        rand_robot_index = rand()%top_brains.size();

        //To SPEED THIS UP, I could only store the brains of the top bots or memoize them
        add_robot(0,0,top_brains[rand_robot_index], top_ancestors[rand_robot_index]);
    }

    
    
    
    

}

void World::step(){ 
    handle_robot_garden_collisions();
    //Make the robots step and then I think we will be don :)
    for( auto it=robot_vec.begin(); it!=robot_vec.end(); it++ ){
        (*it)->step();
    }
    if(time(NULL)-last_mutate_time > mutate_time){
        mutate(.3);
        last_mutate_time = time(NULL);
    }
}




