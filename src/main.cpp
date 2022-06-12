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
 
int main(int argc, char** argv) {
    SDL_Window* window = NULL;
    SDL_Surface* surface = NULL;

    bool quit = false;

    // Event handler
    SDL_Event e;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError();
    } else {
        window = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if (window == NULL) {
            std::cout << "Window could not be created! SDL_Error " << SDL_GetError();
        }

        surface = SDL_GetWindowSurface(window);
        //Fill the surface white
        SDL_FillRect(surface, NULL, SDL_MapRGB( surface->format, 0xFF, 0xFF, 0xFF ) );
           
        //Update the surface
        SDL_UpdateWindowSurface( window );

        //Wait two seconds
        SDL_Delay( 2000 );

        //Destroy window
        SDL_DestroyWindow( window );

        //Quit SDL subsystems
        SDL_Quit();
    }

    return 0;
}

bool init() {
    // TODO: continue from here 
    // http://lazyfoo.net/tutorials/SDL/02_getting_an_image_on_the_screen/index.php
}



