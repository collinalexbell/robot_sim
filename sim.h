#ifndef SIM_H
#define SIM_H

#include <SDL/SDL.h>
#include <string>
#include <vector>
#include <unordered_map>

class Sim
{
    public:
        Sim(int w, int h);
        void init();
        void quit_sdl();
        SDL_Surface* get_screen();

        //Returns the id for the gui_test
        unsigned int start_gui_test_bool(char* test_text, bool desired_result);

        bool get_gui_test_result(int test_id);

        bool gui_test_finished(unsigned int test_id);

        bool step();

        void draw();

        unsigned int add_drawable(SDL_Surface * surf, int x, int y);

        SDL_Event event;
        SDL_Surface *screen;

    private:
        struct Drawable{
            SDL_Surface* surf;
            SDL_Rect* offset;

           /* ~Drawable(){
                delete surf;
                delete offset;
            }
            */
        };

        int screen_height;
        int screen_width;
        bool running;
        std::vector<bool> gui_tests;
        bool bool_gui_test_finished = false;
        bool bool_gui_result = false;
    public:
        std::unordered_map<unsigned int, Drawable> things_to_draw;

};

#endif
