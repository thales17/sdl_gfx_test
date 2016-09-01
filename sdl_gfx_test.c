#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#define MAZE_LINE_COUNT 500

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

typedef struct point {
  int x;
  int y;
} point;

typedef struct line {
  point p1;
  point p2;
} line;

int fullscreen = 0;
float t = 0.0;
line rLines[100];
line mLines[MAZE_LINE_COUNT];

void boxLines(line lines[], int x, int y, int w, int h);
void randomLines(line lines[], int count, int x, int y, int w, int h);
void mazeLines(line lines[], int count, int x, int y, int w, int h);

void draw(SDL_Window *window, SDL_Renderer *renderer);
void drawLines(line lines[], int numLines, int thickness, int r, int g, int b, int a, SDL_Renderer *renderer);
void drawTest(SDL_Renderer *renderer);

int main(int argc, char *args[]) {
  time_t t;
  SDL_Window *window = NULL;
  SDL_Renderer *renderer = NULL;


  /* Intialize random number generator */
  srand((unsigned) time(&t));
  randomLines(rLines, 100, 200, 200, 100, 100);
  mazeLines(mLines, MAZE_LINE_COUNT, 20, 20, SCREEN_WIDTH - 40, SCREEN_HEIGHT - 40);

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
          t = 0;
          if(!fullscreen) {
            SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
            SDL_DisplayMode mode;
            SDL_GetWindowDisplayMode(window, &mode);
            fullscreen = 1;
            mazeLines(mLines, MAZE_LINE_COUNT, 20, 20, mode.w - 40, mode.h - 40);
          } else {
            SDL_SetWindowFullscreen(window, 0);
            fullscreen = 0;
            mazeLines(mLines, MAZE_LINE_COUNT, 20, 20, SCREEN_WIDTH - 40, SCREEN_HEIGHT - 40);
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
  int w = SCREEN_WIDTH;
  int h = SCREEN_HEIGHT;
  if(fullscreen) {
    SDL_GetWindowDisplayMode(window, &mode);
    w = mode.w;
    h = mode.h;
  }
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);
  
  //drawTest(renderer);

  line bLines[4];
  boxLines(bLines, 10, 10, w - 20, h - 20);
  //drawLines(bLines, 4, 2, 0, 0, 255, 255, renderer);

  boxLines(bLines, 100, 100, 100, 100);
  //drawLines(bLines, 4, 5, 255, 0, 0, 255, renderer);

  //drawLines(rLines, 100, 3, 255, 0, 0, 50, renderer);
  
  drawLines(mLines, 500, 3, 0, 255, 0, 40, renderer);

  SDL_RenderPresent(renderer);
  
  t += 1.0 / 480.0;
  if(t > 1) {
    t = 0;
    mazeLines(mLines, MAZE_LINE_COUNT, 20, 20, w - 40, h - 40);
  }
}

void drawTest(SDL_Renderer *renderer) {
  SDL_Rect rect;
  rect.x = 20;
  rect.y = 20;
  rect.w = 20;
  rect.h = 20;

  SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
  SDL_RenderFillRect(renderer, &rect);

  filledCircleColor(renderer, 50, 50, 10, 0xFF0000FF);
  aacircleColor(renderer, 50, 50, 10, 0xFF0000FF);

  thickLineRGBA(
      renderer,
      100,
      100,
      130,
      130,
      3,
      0,
      255,
      0,
      255
  );
}

void boxLines(line lines[], int x, int y, int w, int h) {
  line l1;
  l1.p1.x = x;
  l1.p1.y = y;
  l1.p2.x = x + w;
  l1.p2.y = y;

  line l2;
  l2.p1.x = x + w;
  l2.p1.y = y;
  l2.p2.x = x + w;
  l2.p2.y = y + h;

  line l3;
  l3.p1.x = x + w;
  l3.p1.y = y + h;
  l3.p2.x = x;
  l3.p2.y = y + h;

  line l4;
  l4.p1.x = x;
  l4.p1.y = y + h;
  l4.p2.x = x;
  l4.p2.y = y;

  lines[0] = l1;
  lines[1] = l2;
  lines[2] = l3;
  lines[3] = l4;
}

void randomLines(line lines[], int count, int x, int y, int w, int h) {
  point lastPoint;
  lastPoint.x = x + rand() % (w - x);
  lastPoint.y = y + rand() % (h - y);
  for(int i = 0; i < count; i++) {
    line l;
    l.p1 = lastPoint;
    int moveX = rand() % 2;
    if(moveX) {
      l.p2.x = x + rand() % (w - x);
      l.p2.y = l.p1.y;
    } else {
      l.p2.x = l.p1.x;
      l.p2.y = y + rand() % (h - y);
    }
    lines[i] = l;
    lastPoint = l.p2;
  }
}

void mazeLines(line lines[], int count, int x, int y, int w, int h) {
  point lastPoint;
  lastPoint.x = x;
  lastPoint.y = y;

  int xDir = 1;
  int yDir = 1;

  for(int i = 0; i < count; i++) {
    line l;
    l.p1 = lastPoint;
    int moveX = rand() % 2;
    if(moveX) {
      l.p2.x = l.p1.x + (rand() % (w / 10) * xDir);
      l.p2.y = l.p1.y;
    } else {
      l.p2.x = l.p1.x;
      l.p2.y = l.p1.y + (rand() % (h / 10) * yDir);
    }
    lines[i] = l;
    lastPoint = l.p2;

    if(l.p2.x + xDir * (w / 10) > (x + w) || l.p2.x + xDir * (w / 10) < x) {
      xDir *= -1;
    }

    if(l.p2.y + yDir * (h / 10) > (y + h) || l.p2.y + yDir * (h / 10) < y) {
      yDir *= -1;
    }
  }
}

void drawLines(line lines[], int numLines, int thickness, int r, int g, int b, int a, SDL_Renderer *renderer) {
  
  for(int i = 0; i < numLines; i++) {
    float segT = 1.0 / (float)numLines;
    int segIndex = (int) (t / segT);
    point p1 = lines[i].p1;
    point p2 = lines[i].p2;
    //printf("segT: %f\n", segT);
    //printf("segIndex: %d\n", segIndex);
    
    if(i <= segIndex) {
      if(i == segIndex) {
        float localT = (float)fmod(t, segT);
        float p = localT / segT;
        //printf("localT: %f\n", localT);
        //printf("p: %f\n", p);
        p2.x = p1.x + (int)roundf((p2.x - p1.x) * p);
        p2.y = p1.y + (int)roundf((p2.y - p1.y) * p);
      }
      
      if(thickLineRGBA(
          renderer,
          p1.x, p1.y,
          p2.x, p2.y,
          thickness,
          r, g, b, a
      ) != 0) {
        printf("thickLineColor error %s", SDL_GetError());
      }
    }
  }
}
