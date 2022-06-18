// http://lazyfoo.net/tutorials/SDL/02_getting_an_image_on_the_screen/index.php
#include <iostream>
#include <string>
#include <assert.h>
#include <SDL2/SDL.h>
#include "chip8.cpp"

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The surface contained by the window
SDL_Surface* gScreenSurface = NULL;

//The image we will load and show on the screen
SDL_Surface* gHelloWorld = NULL;

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

void print_SDL_error();
 
int main(int argc, char** argv) {
    if (!init()) {
        std::cout << "Failed to initialize" << std::endl;

    } else {
        if (!loadMedia()) {
            std::cout << "Failed to load media" << std::endl;
        } else {
            // Blit updates back buffer
            SDL_BlitSurface(gHelloWorld, NULL, gScreenSurface, NULL);
            
            // IF thisline code not run, image will not appear
            // this updates the front buffer
            SDL_UpdateWindowSurface(gWindow);
        }
        SDL_Delay(6000);
    }

    close();

    return 0;
}

bool init() {
    bool success = true;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        print_SDL_error();
        success = false;
    } else {
        gWindow = SDL_CreateWindow("SDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow == NULL) {
            print_SDL_error();
            success = false;
        } else {
            gScreenSurface = SDL_GetWindowSurface(gWindow);
        }
    }

    return success;
}

bool loadMedia() {
    bool success = true;

    gHelloWorld = SDL_LoadBMP("/home/zhenhao-ng/sample_640×426.bmp");
    if (gHelloWorld == NULL) {
        std::cout << "Unable to load image " << SDL_GetError() << std::endl;
        success = false;
    }  
    return success;
}

void close() {
    SDL_FreeSurface(gHelloWorld);
    gHelloWorld = NULL;

    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    SDL_Quit();
}

void print_SDL_error() {
    std::cout << "SDL not initialzied " << SDL_GetError() << std::endl;
}



