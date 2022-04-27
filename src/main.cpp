#include "SDL.h"
#include <iostream>

#ifndef __ANDROID__
#include "glad/gl.h"
#else
#include <GLES3/gl3.h>
#define glClearDepth(color) glClearDepthf(color)
#endif

SDL_Window *window = nullptr;
SDL_GLContext glContext = nullptr;

void renderFrame();

int main(int argc, char **argv) {
  SDL_Init(SDL_INIT_EVERYTHING);

  window =
      SDL_CreateWindow("CrossGui Window", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);

  glContext = SDL_GL_CreateContext(window);

#ifndef __ANDROID__
  gladLoaderLoadGL();
#endif

  glClearDepth(1.0);

  glEnable(GL_DEPTH_TEST);
  SDL_GL_SetSwapInterval(1);

  bool running = true;
  while (running) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_QUIT:
        running = false;
        break;

      case SDL_KEYDOWN: {
        if (event.key.keysym.sym == SDLK_ESCAPE) {
          running = false;
        }
        break;
      }

      default:
        break;
      }
    }

    renderFrame();
  }

  SDL_Quit();

  return 0;
}

void renderFrame() {
  glClearColor(0.12, 0.44, 0.87, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  SDL_GL_SwapWindow(window);
}