#ifndef SIM_H
#define SIM_H

#include <SDL/SDL.h>
#include <string>
#include <vector>

class Sim
{
    public:
        Sim(int w, int h);
        void init();
        void quit_sdl();
        SDL_Surface* get_screen();

        //Returns the id for the gui_test
        int start_gui_test_bool(char* test_text, bool desired_result);

        bool get_gui_test_result(int test_id);

        bool gui_test_finished(int test_id);

        bool step();

        void draw();

    private:
        struct Drawable{
            SDL_Surface* surf;
            SDL_Rect offset;
        };

        int screen_height;
        int screen_width;
        SDL_Surface *screen;
        bool running;
        std::vector<bool> gui_tests;
        SDL_Event event;
        bool bool_gui_test_finished = false;
        bool bool_gui_result = false;
        std::vector<Drawable> things_to_draw;

};

#endif
