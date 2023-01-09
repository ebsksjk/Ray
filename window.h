///////////////////////////////////////////////////////////////////////////////////////////////////
// hella hella hey
//  
///////////////////////////////////////////////////////////////////////////////////////////////////

#include <SDL2/SDL.h>

SDL_Window* window      = NULL;
SDL_Renderer* renderer  = NULL;

int init(void){
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL failed to initialise: %s\n", SDL_GetError());
        return -1;
    }

    return 0;
}

int createWindow(void){
    window = SDL_CreateWindow("hehe", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, 0);

    if (window == NULL) {
        fprintf(stderr, "SDL window failed to initialise: %s\n", SDL_GetError());
        return -2;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    return 0;
}