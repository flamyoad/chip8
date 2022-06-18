// http://lazyfoo.net/tutorials/SDL/02_getting_an_image_on_the_screen/index.php
#include <iostream>
#include <string>
#include <assert.h>
#include <SDL2/SDL.h>
#include "chip8.cpp"

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

enum KeyPressSurfaces {
	KEY_PRESS_SURFACE_DEFAULT,
	KEY_PRESS_SURFACE_UP,
	KEY_PRESS_SURFACE_DOWN,
	KEY_PRESS_SURFACE_LEFT,
	KEY_PRESS_SURFACE_RIGHT,
	KEY_PRESS_SURFACE_TOTAL
};

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The surface contained by the window
SDL_Surface* gScreenSurface = NULL;

//The images that correspond to a keypress
SDL_Surface* gKeyPressSurfaces[KEY_PRESS_SURFACE_TOTAL];

//Current displayed image
SDL_Surface* gCurrentSurface = NULL;

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//Loads individual image
SDL_Surface* load_surface(const std::string &path);

void print_SDL_error(const std::string &s = "");

int main(int argc, char** argv) {
    if (!init()) {
        std::cout << "Failed to initialize" << std::endl;

    } else {
        if (!loadMedia()) {
            std::cout << "Failed to load media" << std::endl;
        } else {

            bool quit = false;

            gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];

            SDL_Event e;
            while (!quit) {
                while (SDL_PollEvent(&e) != 0) {
                    if (e.type == SDL_QUIT) {
                        quit = true;
                    } else if (e.type == SDL_KEYDOWN) {
                        switch (e.key.keysym.sym) {
                            case SDLK_UP:
                                gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_UP];
                            break;

                            case SDLK_DOWN:
                                gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN];
                            break; 

                            case SDLK_LEFT:
                                gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_LEFT ];
                            break;

                            case SDLK_RIGHT:
                                gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_RIGHT ];
                            break;

                            default:
                                gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ];
                            break;
                        }
                    }
                }
			    //Apply the current image
			    SDL_BlitSurface( gCurrentSurface, NULL, gScreenSurface, NULL );
			
			    //Update the surface
			    SDL_UpdateWindowSurface( gWindow );
            }
        }
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

    gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] = SDL_LoadBMP("../src/press.bmp");
    if (gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] == NULL) {
        print_SDL_error("Failed load default");
        success = false;
    }  

    gKeyPressSurfaces[KEY_PRESS_SURFACE_UP] = SDL_LoadBMP("../src/up.bmp");
    if (gKeyPressSurfaces[KEY_PRESS_SURFACE_UP] == NULL) {
        print_SDL_error("Failed load up");
        success = false;
    }  

    gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN] = SDL_LoadBMP("../src/down.bmp");
    if (gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN] == NULL) {
        print_SDL_error("Failed load down");
        success = false;
    }  

    gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT] = SDL_LoadBMP("../src/left.bmp");
    if (gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT] == NULL) {
        print_SDL_error("Failed load left");
        success = false;
    }  

    gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] = SDL_LoadBMP("../src/right.bmp");
    if (gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] == NULL) {
        print_SDL_error("Failed load right");
        success = false;
    }  
    return success;
}

void close() {
    for (int i = 0; i < KEY_PRESS_SURFACE_TOTAL; ++i) {
        SDL_FreeSurface(gKeyPressSurfaces[i]);
        gKeyPressSurfaces[i] = NULL;
    }

    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    SDL_Quit();
}

SDL_Surface* load_surface(const std::string &path) {
    SDL_Surface* loaded_surface = SDL_LoadBMP(path.c_str());
    if (loaded_surface == NULL) {
        print_SDL_error("Unable to load image " + path);
        return NULL;
    }
    return loaded_surface;
}

// Default argument has to go to function prototypr only
void print_SDL_error(const std::string &s) {
    if (s.empty()) {
        std::cout << SDL_GetError() << std::endl;
    } else {
        std::cout << s << " " << SDL_GetError() << std::endl;
    }
}



