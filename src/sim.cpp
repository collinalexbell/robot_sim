#include "sim.h"
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <string>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "drawable.h"

#define MAX_ID 65535

Sim::Sim(int w, int h){
    screen_width = w;
    screen_height = h;
}

SDL_Surface* Sim::get_screen(){
    return screen;
}

void Sim::init(){
    srand (time(NULL));
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    screen = SDL_SetVideoMode(screen_width, screen_height, 32, SDL_SWSURFACE);
    running = true;

#ifdef TEST_SDL_LOCK_OPTS
    EM_ASM("SDL.defaults.copyOnLock = false; SDL.defaults.discardOnLock = true; SDL.defaults.opaqueFrontBuffer = false;");
#endif

    if (SDL_MUSTLOCK(screen)) SDL_LockSurface(screen);
    for (int i = 0; i < 256; i++) {
    for (int j = 0; j < 256; j++) {
#ifdef TEST_SDL_LOCK_OPTS
        // Alpha behaves like in the browser, so write proper opaque pixels.
        int alpha = 255;
#else
        // To emulate native behavior with blitting to screen, alpha component is ignored. Test that it is so by outputting
        // data (and testing that it does get discarded)
        int alpha = (i+j) % 255;
#endif
        *((Uint32*)screen->pixels + i * 256 + j) = SDL_MapRGBA(screen->format, i, j, 255-i, alpha);
    }
    }
    if (SDL_MUSTLOCK(screen)) SDL_UnlockSurface(screen);
    SDL_Flip(screen); 

}

void Sim::quit_sdl(){
    SDL_Quit();
}

unsigned int Sim::start_gui_test_bool(char test_text[], bool desired_result){
    bool_gui_test_finished = false;
    int rv = 0;
    TTF_Font *font = NULL;
    SDL_Surface *message = NULL;
    SDL_Surface *message2 = NULL;
    SDL_Surface *full_message = SDL_CreateRGBSurface(0,1080, 40, 32,0,0,0,0);
    SDL_Rect* yes_no_offset = new SDL_Rect();
    SDL_Rect* full_message_offset = new SDL_Rect();


    yes_no_offset->y = 20;
    yes_no_offset->x = 0;

    SDL_Color textColor = { 255, 255, 255 };
    font = TTF_OpenFont( "opensans.ttf", 14 );
    message = TTF_RenderText_Solid( font, test_text, textColor );
    std::cout << SDL_GetError();
    message2 = TTF_RenderText_Solid( font, "Press Y/N", textColor );
    std::cout << SDL_GetError();
    int m1res = SDL_BlitSurface(message, NULL, full_message, NULL);
    int m2res = SDL_BlitSurface(message2, NULL, full_message, yes_no_offset);
    char buffer [60];
    std::sprintf(buffer,"message1 result: %d, message2 result: %d",m1res, m2res);

    Point p =  {0,720};
    Basic_Drawable* gui_message = new Basic_Drawable(full_message, p);

    //UUID max is max of unsigned int
    unsigned int uuid = rand() % MAX_ID;
    std::pair<unsigned int, Drawable*> insert_me;
    insert_me = std::make_pair(uuid,gui_message);
    things_to_draw.insert(insert_me);

    return uuid;
}

void Sim::draw(){
    SDL_FillRect(screen, NULL, 0x000000);
    for ( auto it = things_to_draw.begin(); it != things_to_draw.end(); ++it ){

        //Offset from drawable
        SDL_Rect *offset = new SDL_Rect();
        offset->x = it->second->get_position().x;
        offset->y = it->second->get_position().y;
        
        //Surf from drawable
        SDL_Surface* surf = it->second->get_image();

        SDL_BlitSurface(surf, NULL, screen, offset);
    } 
}

bool Sim::step(){
    if( SDL_PollEvent( &event ) ) {
        //If the user has Xed out the window 
        if( event.type == SDL_QUIT ) {
            //Quit the program
            return false;
        }
        if(event.type == SDL_KEYDOWN){
            if(event.key.keysym.sym == SDLK_y){
                bool_gui_test_finished = true;
                bool_gui_result = true;
            }
            if( event.key.keysym.sym == SDLK_n){
                bool_gui_test_finished = true;
                bool_gui_result = false;
            }
        }
    }
    draw();
    SDL_Flip(screen);
    return true;
}

bool Sim::gui_test_finished(unsigned int test_id){
    if(bool_gui_test_finished == true){
        things_to_draw.erase(test_id);
    }
    return bool_gui_test_finished;
}

bool Sim::get_gui_test_result(int test_id){
    return bool_gui_result;
}

unsigned int Sim::add_drawable(Drawable* drawable){


    unsigned int uuid = rand() % MAX_ID;

    std::pair<unsigned int, Drawable*> insert_me (uuid, drawable);
    things_to_draw.insert(insert_me);
    std::cout<< things_to_draw.size() << std::endl;

    return uuid;
}
