#include "SDL.h"

#include <iostream>

#ifdef __ANDROID__
#include <GLES3/gl3.h>
#define glClearDepth glClearDepthf
#else
#include "glad/gl.h"
#endif

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"

#include "fonts/IconsFontAwesome6.h"
#include "fonts/IconsFontAwesome6.h_fa-solid-900.ttf.h"
#include "fonts/CaskaydiaCoveNerdFont.h"

SDL_Window *window = nullptr;
SDL_GLContext glContext = nullptr;

void drawUi();
void renderFrame();

static constexpr char *imgui_settings_file_name = "imgui.ini";
bool load_imgui_settings_file();
bool save_imgui_settings_file();

int main(int argc, char **argv)
{
    SDL_Init(SDL_INIT_EVERYTHING);

    window =
        SDL_CreateWindow("CrossGui Window", SDL_WINDOWPOS_CENTERED,
                         SDL_WINDOWPOS_CENTERED, 600, 800, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    glContext = SDL_GL_CreateContext(window);

#ifndef __ANDROID__
    gladLoaderLoadGL();
#endif

    glClearDepth(1.0);

    glEnable(GL_DEPTH_TEST);
    SDL_GL_SetSwapInterval(1);

    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();

    load_imgui_settings_file();

    ImGui_ImplSDL2_InitForOpenGL(window, glContext);
    ImGui_ImplOpenGL3_Init();

#ifdef __ANDROID__
    static constexpr float font_size = 24;
#else
    static constexpr float font_size = 16;
#endif

    constexpr ImWchar icon_ranges[] = {ICON_MIN_FA, ICON_MAX_FA, 0};
    ImFontConfig nerdFontConfig{};
    nerdFontConfig.MergeMode = true;
    auto *nerdFont = io.Fonts->AddFontFromMemoryCompressedTTF(caskaydia_cove_nerd_bin_compressed_data, font_size, 20, nullptr, io.Fonts->GetGlyphRangesDefault());
    auto *fontAwesome = io.Fonts->AddFontFromMemoryTTF((void *)s_fa_solid_900_ttf, font_size, 20, &nerdFontConfig, icon_ranges);

    bool running = true;

    bool recTextInput = false;
    while (running)
    {
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (io.WantTextInput)
            {
                if (!recTextInput)
                {
                    recTextInput = true;
                    SDL_StartTextInput();
                }
            }
            else
            {
                SDL_StopTextInput();
                recTextInput = false;
            }

            switch (event.type)
            {
            case SDL_QUIT:
                running = false;
                break;

            case SDL_KEYDOWN:
            {
                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    running = false;
                }
                break;
            }

            case SDL_MOUSEBUTTONDOWN:
            {
                break;
            }

            default:
                break;
            }
        }
        drawUi();
        renderFrame();
    }

    save_imgui_settings_file();

    SDL_Quit();

    return 0;
}

ImVec4 clearColor = {0.5, 0.2, 0.1, 1.0};

void renderFrame()
{
    ImGui::Render();

    glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(window);
}

void drawUi()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    {
        static char inputText[128] = "";
        ImGui::Begin("Hello Window");

        ImGui::Text("FontAwesome Icons -> " ICON_FA_GEAR ICON_FA_BARS);
        ImGui::InputText("Some Text Input", inputText, 128);
        ImGui::ColorPicker4("GL Clear Color", (float *)&clearColor);

        ImGui::End();
    }
}

bool load_imgui_settings_file()
{
    auto *iniFileIn = SDL_RWFromFile(imgui_settings_file_name, "r+b");
    if (iniFileIn == nullptr)
        return false;

    char iniBuff[4096]{0};
    size_t readSize = SDL_RWread(iniFileIn, iniBuff, sizeof(char), 4096);
    if (readSize == 0)
        return false;

    ImGui::LoadIniSettingsFromMemory(iniBuff, readSize);
    SDL_RWclose(iniFileIn);

    return true;
}

bool save_imgui_settings_file()
{
    size_t iniSize = 0;
    auto *iniData = ImGui::SaveIniSettingsToMemory(&iniSize);

    auto *iniFile = SDL_RWFromFile(imgui_settings_file_name, "w+b");
    if (iniFile == nullptr)
        return false;

    if (SDL_RWwrite(iniFile, iniData, sizeof(char), iniSize) < iniSize)
        return false;

    SDL_RWclose(iniFile);
    return true;
}