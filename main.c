///////////////////////////////////////////////////////////////////////////////////////////////////
// main.c - hier ist das ganze tolle Zeug wie die Mainloop
// sky: 168, 190, 255
// floor: 115, 103, 254
///////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdbool.h>
#include <stdio.h>

#include <SDL2/SDL.h>

///////////////////////////////////////////////////////////////////////////////////////////////////

// die konstanten für das Fenster (resizing ist langweilig)
unsigned const int WIDTH = 800;
unsigned const int HEIGHT = 600;

///////////////////////////////////////////////////////////////////////////////////////////////////

#include "raycaster.h"
#include "window.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

void handleKeys(SDL_KeyCode keycode);

///////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char **argv) {

  init();
  createWindow();

  bool done;
  SDL_Event event;
  SDL_Rect floor;

  floor.x = 0;
  floor.y, floor.h = HEIGHT / 2;
  floor.w = WIDTH;

  while ((!done) && (SDL_WaitEvent(&event))) {

    switch (event.type) {

    case SDL_KEYDOWN:
      handleKeys(event.key.keysym.sym);
      break;

    case SDL_QUIT:
      done = true;
      break;

    default:
      break;
    }

    // wir zeichnen den Himmel ein, indem wir den Hintergrund füllen
    SDL_SetRenderDrawColor(renderer, 168, 190, 255, 255);
    SDL_RenderClear(renderer);

    // wir malen ein Rechteck für den Boden
    SDL_SetRenderDrawColor(renderer, 115, 103, 254, 255);
    SDL_RenderFillRect(renderer, &floor);

    // wir rufen den raycaster auf und lassen ihn in den Buffer schreiben
    raycast(renderer);

    // zum Schluss wechseln wir die Windowbuffer aus
    SDL_RenderPresent(renderer);
  }

  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void handleKeys(SDL_KeyCode keycode) {
  switch (keycode) {
  case SDLK_RIGHT:
    rotate(-1);
    break;

  case SDLK_LEFT:
    rotate(1);
    break;

  case SDLK_UP:
    forwards();
    break;

  case SDLK_DOWN:
    backwards();
    break;

  /*case SDLK_ESCAPE:
    done = true;
    break;
  */
  default:
    break;
  }
}