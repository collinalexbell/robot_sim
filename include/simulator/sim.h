#ifndef SIM_H
#define SIM_H

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <string>
#include <vector>
#include <unordered_map>
#include "drawable.h"
#include "world.h"

class Sim
{
    public:
        Sim(int w, int h);
        void init();
        void quit_sdl();
        SDL_Surface* get_screen();

        //Returns the id for the gui_test
        unsigned int start_gui_test_bool(char test_text[], bool desired_result);

        bool get_gui_test_result(int test_id);

        bool gui_test_finished(unsigned int test_id);

        bool step();

        void draw();

        void make_world(int num_robots, int num_gardens);

        void draw_fps();

        unsigned int add_drawable(Drawable* drawable);

        SDL_Event event;
        SDL_Surface *screen;

    private:
        int screen_height;
        int screen_width;
        bool running;
        std::vector<bool> gui_tests;
        bool bool_gui_test_finished = false;
        bool bool_gui_result = false;
        World* world = NULL;
        unsigned int last_step = 0;
        unsigned int last_fps_print = 0;
        int tick = 0;
        int tick_till_print = 0;
        double* fps = new double[10];
        double fps_to_print;
        TTF_Font *simFont;
        SDL_Color simTextColor;
        SDL_Rect *fps_offset;

    public:
        std::unordered_map<unsigned int, Drawable*> things_to_draw;

};

#endif
