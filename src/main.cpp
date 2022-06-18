#include <iostream>
#include <string>
#include <chrono>
#include <SDL.h>
#include "chip8.cpp"

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

const int TEXTURE_WIDTH = 640;
const int TEXTURE_HEIGHT = 480;

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Texture *texture;
SDL_Event event;

bool initialize_window();
bool accept_input(uint8_t *);
void update_frame(void const *, int);
void log_SDL_error(const std::string &s = "");    
void close();
 
int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <ROM>\n";
        std::exit(EXIT_FAILURE);
    }

    char const *rom_path = argv[1];

    Chip8 *chip8 = new Chip8();
    chip8->init();
    chip8->load_rom(rom_path);

    // Should be defined by user in argc. Hard code it for now.
    int cycle_delay = 10;

    int video_pitch = sizeof(chip8->display[0]) * SCREEN_WIDTH;

    initialize_window();

	auto last_cycle_time = std::chrono::high_resolution_clock::now();
    bool quit = false;

    while (!quit) {
        quit = accept_input(chip8->keypad);

        auto current_time = std::chrono::high_resolution_clock::now();
		float dt = std::chrono::duration<float, std::chrono::milliseconds::period>(current_time - last_cycle_time).count();

        if (dt > cycle_delay) {
            last_cycle_time = current_time;
            chip8->emulate_cycle();
            update_frame(chip8->display, video_pitch);
        }
    }

    close();
    return 0;
}

// abstract into a class 
bool initialize_window() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        log_SDL_error("SDL_Init has failed");
        return false;
    }

    window = SDL_CreateWindow("Chip8 Emu", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);    
    if (window == NULL) {
        log_SDL_error("Failed to create window");
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        log_SDL_error("Failed to create renderer");
        return false;
    }

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, TEXTURE_WIDTH, TEXTURE_HEIGHT);
    if (texture == NULL) {
        log_SDL_error("Failed to create texture");
        return false;
    }

    return true;
}

void update_frame(void const *buffer, int pitch) {
	SDL_UpdateTexture(texture, nullptr, buffer, pitch);
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, texture, nullptr, nullptr);
	SDL_RenderPresent(renderer);
}

// Param is array of uint8_t
bool accept_input(uint8_t *keys) {
    bool quit = false;
    SDL_PollEvent(&event);

    switch(event.type) {
        case SDL_QUIT: {
            quit = true;
        } break;

        case SDL_KEYDOWN: {
            switch(event.key.keysym.sym) {
                case SDLK_ESCAPE: 
                    quit = true;
                break;

                case SDLK_x: 
                    keys[0] = 1;
                break;

                case SDLK_1:
                    keys[1] = 1;
                break;

                case SDLK_2:
                    keys[2] = 1;
                break;

                case SDLK_3:
                    keys[3] = 1;
                break;
                
                case SDLK_q :
                    keys[4] = 1;
                break;
          
                case SDLK_w :
                    keys[5] = 1;
                break;
                                
                case SDLK_e :
                    keys[6] = 1;
                break;
                                
                case SDLK_a :
                    keys[7] = 1;
                break;
                                
                case SDLK_s :
                    keys[8] = 1;
                break;
                                
                case SDLK_d :
                    keys[9] = 1;
                break;
                                
                case SDLK_z :
                    keys[0xA] = 1;
                break;
                                
                case SDLK_c :
                    keys[0xB] = 1;
                break;
                                
                case SDLK_4 :
                    keys[0xC] = 1;
                break;
                                
                case SDLK_r :
                    keys[0xD] = 1;
                break;
                                                
                case SDLK_f :
                    keys[0xE] = 1;
                break;
                                                
                case SDLK_v :
                    keys[0xF] = 1;
                break;
            }
        } break;

        case SDL_KEYUP: {
                case SDLK_ESCAPE: 
                    quit = true;
                break;

                case SDLK_x: 
                    keys[0] = 0;
                break;

                case SDLK_1:
                    keys[1] = 0;
                break;

                case SDLK_2:
                    keys[2] = 0;
                break;

                case SDLK_3:
                    keys[3] = 0;
                break;
                
                case SDLK_q :
                    keys[4] = 0;
                break;
          
                case SDLK_w :
                    keys[5] = 0;
                break;
                                
                case SDLK_e :
                    keys[6] = 0;
                break;
                                
                case SDLK_a :
                    keys[7] = 0;
                break;
                                
                case SDLK_s :
                    keys[8] = 0;
                break;
                                
                case SDLK_d :
                    keys[9] = 0;
                break;
                                
                case SDLK_z :
                    keys[0xA] = 0;
                break;
                                
                case SDLK_c :
                    keys[0xB] = 0;
                break;
                                
                case SDLK_4 :
                    keys[0xC] = 0;
                break;
                                
                case SDLK_r :
                    keys[0xD] = 0;
                break;
                                                
                case SDLK_f :
                    keys[0xE] = 0;
                break;
                                                
                case SDLK_v :
                    keys[0xF] = 0;
                break;
        }
        break;
    }
    return quit;
}

void close() {
    SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void log_SDL_error(const std::string &s) {
    if (s.empty()) {
        std::cout << SDL_GetError() << std::endl;
    } else {
        std::cout << s << " " << SDL_GetError() << std::endl;
    }
}

