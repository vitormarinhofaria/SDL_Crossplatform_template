#include "SDL.h"
#include <iostream>

#ifndef __ANDROID__
#include "glad/gl.h"
#else
#include <GLES3/gl3.h>
#define glClearDepth(color) glClearDepthf(color)
#endif

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"

SDL_Window *window = nullptr;
SDL_GLContext glContext = nullptr;

void renderFrame();
void drawUi();

int main(int argc, char **argv) {
  SDL_Init(SDL_INIT_EVERYTHING);

  window =
      SDL_CreateWindow("CrossGui Window", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, 600, 800, SDL_WINDOW_OPENGL);

  glContext = SDL_GL_CreateContext(window);

#ifndef __ANDROID__
  gladLoaderLoadGL();
#endif

  glClearDepth(1.0);

  glEnable(GL_DEPTH_TEST);
  SDL_GL_SetSwapInterval(1);

  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  
  auto* iniFileIn = SDL_RWFromFile("imgui.ini", "r+b");
  if(iniFileIn != nullptr){
    //SDL_RWread()
    char iniBuff[4096]{0};
    size_t readSize = SDL_RWread(iniFileIn, iniBuff, sizeof(char), 4096);
    ImGui::LoadIniSettingsFromMemory(iniBuff, readSize);
    SDL_RWclose(iniFileIn);
  }
  
  ImGui_ImplSDL2_InitForOpenGL(window, glContext);
  ImGui_ImplOpenGL3_Init();

  bool running = true;
  while (running) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
      ImGui_ImplSDL2_ProcessEvent(&event);

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

      case SDL_MOUSEBUTTONDOWN: {
        break;
      }

      default:
        break;
      }
    }
    drawUi();
    renderFrame();
  }
  size_t iniSize = 0;
  auto* iniData = ImGui::SaveIniSettingsToMemory(&iniSize);
  auto* iniFile = SDL_RWFromFile("imgui.ini", "w+b");
  if (iniFile != nullptr){
    SDL_RWwrite(iniFile, iniData, sizeof(char), iniSize);
    SDL_RWclose(iniFile);
  }

  //ImGui_ImplSDL2
  SDL_Quit();

  return 0;
}

ImVec4 clearColor = {0.5, 0.2, 0.1, 1.0};

void renderFrame() {
  ImGui::Render();

  glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  SDL_GL_SwapWindow(window);
}

char inputText[128] = "Text Default.";
void drawUi() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplSDL2_NewFrame();
  ImGui::NewFrame();

  {
    ImGui::Begin("Hello Window", NULL, ImGuiWindowFlags_AlwaysAutoResize);
#ifdef __ANDROID__
    ImGui::SetWindowFontScale(4);
#endif

    ImGui::Text("Textinho doido");
    ImGui::InputText("Entre um texto", inputText, 128);
    ImGui::ColorPicker4("Clear Color", (float *)&clearColor);
    ImGui::End();
  }
}