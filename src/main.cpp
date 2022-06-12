#include <iostream>
#include <string>
#include <assert.h>
#include <SDL2/SDL.h>
#include "chip8.cpp"

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

void sdl2_init();
 
int main(int argc, char** argv) {
    Chip8 *chip8 = new Chip8();
    chip8->init();

    sdl2_init();

    return 0;
}

void sdl2_init() {
    SDL_Window* window = NULL;
    SDL_Surface* surface = NULL;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError();
    }
}

