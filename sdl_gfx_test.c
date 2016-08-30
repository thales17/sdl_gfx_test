#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

int fullscreen = 0;
float t = 0.0;

typedef struct point {
  int x;
  int y;
} point;

void draw(SDL_Window *window, SDL_Renderer *renderer);
void drawRandomLines(SDL_DisplayMode mode, SDL_Renderer *renderer);
void drawTest(SDL_DisplayMode mode, SDL_Renderer *renderer);

int main(int argc, char *args[]) {
  time_t t;
  SDL_Window *window = NULL;
  SDL_Renderer *renderer = NULL;


  /* Intialize random number generator */
  srand((unsigned) time(&t));

  window = SDL_CreateWindow(
      "sdl_gfx_test",
      SDL_WINDOWPOS_UNDEFINED,
      SDL_WINDOWPOS_UNDEFINED,
      SCREEN_WIDTH,
      SCREEN_HEIGHT,
      SDL_WINDOW_SHOWN
  );

  if(window == NULL) {
    printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
  }

  renderer = SDL_CreateRenderer(
      window,
      -1,
      SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
  );

  if(renderer == NULL) {
    printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
  }
  
  while(1) {
    SDL_Event event;
    if(SDL_PollEvent(&event)) {
      if(event.type == SDL_QUIT) {
        break;
      } else if(event.type == SDL_KEYDOWN) {
        if(event.key.keysym.sym == SDLK_q) {
          break;
        } else if(event.key.keysym.sym == SDLK_f) {
          if(!fullscreen) {
            SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
            fullscreen = 1;
          } else {
            SDL_SetWindowFullscreen(window, 0);
            fullscreen = 0;
          }
        }
      }
    }

    draw(window, renderer);
  }
  
  SDL_DestroyRenderer(renderer);  
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}

void draw(SDL_Window *window, SDL_Renderer *renderer) {
  SDL_DisplayMode mode;

  SDL_GetWindowDisplayMode(window, &mode);
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);
  
  //drawTest(mode, renderer);
  drawRandomLines(mode, renderer);

  SDL_RenderPresent(renderer);

  t += 0.00833;
  t = (t > 1) ? 0 : t;
}

void drawTest(SDL_DisplayMode mode, SDL_Renderer *renderer) {
  SDL_Rect rect;
  rect.x = 20;
  rect.y = 20;
  rect.w = 20;
  rect.h = 20;

  SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
  SDL_RenderFillRect(renderer, &rect);

  filledCircleColor(renderer, 50, 50, 10, 0xFF0000FF);
  aacircleColor(renderer, 50, 50, 10, 0xFF0000FF);
  thickLineColor(
      renderer,
      100,
      100,
      130,
      130,
      3,
      0xFF0000FF
  );
}

void drawRandomLines(SDL_DisplayMode mode, SDL_Renderer *renderer) {
  int padding = 10;
  int thickness = 5;
  Uint32 color = 0xFF0000FF;
  point points[5] = {
    { .x = padding, .y = padding },
    { .x = mode.w - padding, .y = padding },
    {. x = mode.w - padding, .y = mode.h - padding },
    { .x = padding, .y = mode.h - padding },
    { .x = padding, .y = padding }
  };
  point endPoint = {
    .x = points[0].x + (int)roundf((points[1].x - points[0].x) * t),
    .y = points[0].y + (int)roundf((points[1].y - points[0].y) * t)
  };

  thickLineColor(
      renderer,
      points[0].x, points[0].y,
      endPoint.x, endPoint.y,
      thickness,
      color
  );
}
