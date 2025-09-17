#pragma once
#include <chrono>
#include "Scene.h" //unordered_map, Dialogue, Geometry
#include "../Core/Camera.h"
#include "../Graphics/Window.h"
#include "../Graphics/Renderer.h" //SDL_render, Sprite (SDL_image)

using namespace std;
using namespace chrono;
using hr_clock = high_resolution_clock;
using durationf = duration<float>;

class Game {
private:
    //Variables
    uint fps = 0;
    uint game_frames = 0;
    float target_frame_time;
    hr_clock::time_point last_time;
    durationf delta;
    float accumulated_time = 0.f;
    float music_volume = 100;
    float sfx_volume = 100;

public:
    //Game UTH details
    const Vec2u min_res = { 640, 360 };
    Vec2u resolution;
    Window window;
    Renderer renderer;
    bool running = true;
    float delta_time = 0.f;
    bool paused = false;
    Scene scene;

    //Camera
    Camera camera;

    //Music & SFX - waiting for SDL_mixer 3.0
    //DJ dj;
    //Soundboard sb;

    //Miscellaneous
    unordered_map<int, Font> default_fonts;
    Text debug_txt;
    Sprite cursor;

    Game(const char* title, const uint init_fps);
    ~Game() {
        TTF_Quit();
        //Mix_CloseAudio();
        //Mix_Quit();
        SDL_Quit(); //Pretty sure this has to be called last
    }

    //Engine
    void Run();
    void ProcessInput();
    void Update();
    void Render();

    //Frame stuff
    inline uint GetFPS() const { return fps; }
    inline int GetGameFrames() const { return game_frames; }

    //Settings
    void SetMusicVolume(float n_v);
    inline float GetMusicVolume() const { return music_volume; }
    void SetSFXVolume(float n_v);
    inline float GetSFXVolume() const { return sfx_volume; }

    void SetResolution(uchar res_scalar);
    void SetResolution(Vec2u n_r);
    void SetRes();
};