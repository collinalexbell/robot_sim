#include "sim.h"
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <string>
#include <iostream>

Sim::Sim(int w, int h){
    screen_width = w;
    screen_height = h;
}

SDL_Surface* Sim::get_screen(){
    return screen;
}

void Sim::init(){
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    screen = SDL_SetVideoMode(1080, 720, 32, SDL_SWSURFACE);
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

int Sim::start_gui_test_bool(char* test_text, bool desired_result){
    bool_gui_test_finished = false;
    int rv = 1;
    TTF_Font *font = NULL;
    SDL_Surface *message = NULL;
    SDL_Surface *message2 = NULL;
    SDL_Surface *full_message = NULL;
    SDL_Rect yes_no_offset;
    SDL_Rect full_message_offset;


    yes_no_offset.y = 120;
    yes_no_offset.x = 0;
    full_message_offset.x = 0;
    full_message_offset.y = 50;

    SDL_Color textColor = { 255, 255, 255 };
    font = TTF_OpenFont( "opensans.ttf", 14 );
    message = TTF_RenderText_Solid( font, test_text, textColor );
    message2 = TTF_RenderText_Solid( font, "Press Y/N", textColor );
    SDL_BlitSurface(message, NULL, full_message);
    SDL_BlitSurface(message2, NULL, full_message, yes_no_offset);

    
    Drawable drawable = {full_message, offset};
    things_to_draw.push_back(drawable);

    return rv;
}

void Sim::draw(){
    SDL_FillRect(screen, NULL, 0x000000);
    int i;
    for (i = 0; i < things_to_draw.size(); i++){
        SDL_BlitSurface(things_to_draw[i].surf, NULL, screen, &things_to_draw[i].offset);
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

bool Sim::gui_test_finished(int test_id){
    if(bool_gui_test_finished == true;){
        things_to_draw.erase(test_id);
    }
    return bool_gui_test_finished;
}

bool Sim::get_gui_test_result(int test_id){
    return bool_gui_result;
}


