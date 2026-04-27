#pragma once
#include <chrono>
#include <SDL3_ttf/SDL_ttf.h>
#include "Audio/DJ.h" //Enums, SDL_mixer
#include "Graphics/Renderer.h" //SDL_render, Sprite (SDL_image), Text
#include "Graphics/TextureManager.h"
#include "Graphics/Window.h" //SDL, SDL_main, SDL_video
#include "../Game/Game.h" //Sprite

using namespace std;
using namespace chrono;
using hr_clock = steady_clock;
using durationf = duration<float>;

class Core {
private:
    uchar game_frames = 0;
    float fps = 0.f;
    durationf target_frame_time, frame_time, delta_time, accumulated_time;
    hr_clock::time_point now, last_time;
    float sfx_volume = 100;

public:
    const Vec2u min_res = { 640, 360 };
    Vec2i resolution;
    Window window;
    Renderer renderer;
    TextureManager tex_man;
    Game game;
    bool running = true;

    Core(const char* title, const float init_fps);
    ~Core() {
        TTF_Quit();
        MIX_Quit();
        SDL_Quit(); //Has to be called last
    }

    //Engine
    void Run();
    void Render();

    //Frame stuff
    [[nodiscard]] inline float GetFPS() const { return fps; }
    [[nodiscard]] inline uchar GetGameFrames() const { return game_frames; }

    //Settings
    void SetSFXVolume(float n_v);
    [[nodiscard]] inline float GetSFXVolume() const { return sfx_volume; }

    void SetResolution(uchar res_scalar);
    void SetResolution(Vec2u n_r);
    void SetRes();
};