#pragma once
#include <chrono>
#include <SDL3_ttf/SDL_ttf.h>
#include "Audio/DJ.h" //Enums, SDL_mixer
#include "Graphics/Camera.h"//Geometry (Vec2 (iostream))
#include "Graphics/Window.h" //SDL, SDL_main, SDL_video
#include "Graphics/Renderer.h" //SDL_render
#include "../Game/Game.h" //Sprite

using namespace std;
using namespace chrono;
using hr_clock = steady_clock;
using durationf = duration<float>;

class Engine {
private:
    //Variables
    uchar game_frames = 0;
    float fps = 0.f, target_frame_time = 0.f, delta_time = .0f, accumulated_time = .0f;;
    hr_clock::time_point last_time;
    durationf delta;
    float sfx_volume = 100;

public:
    const Vec2u min_res = { 640, 360 };
    Vec2u resolution;
    Window window;
    Renderer renderer;
    DJ dj;
    Camera camera;
    Game game;
    bool running = true;

    Engine(const char* title, const float init_fps);
    ~Engine() {
        TTF_Quit();
        MIX_Quit();
        SDL_Quit(); //Has to be called last
    }

    //Engine
    void Run();
    void Render();

    //Frame stuff
    [[nodiscard]] inline uchar GetFPS() const { return fps; }
    [[nodiscard]] inline uchar GetGameFrames() const { return game_frames; }

    //Settings
    void SetSFXVolume(float n_v);
    [[nodiscard]] inline float GetSFXVolume() const { return sfx_volume; }

    void SetResolution(uchar res_scalar);
    void SetResolution(Vec2u n_r);
    void SetRes();
};