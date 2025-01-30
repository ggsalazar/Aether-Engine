#pragma once
#include <iostream>
#include <memory>
#include <unordered_map>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "../DJ.h"
#include "../Utils/Macros.h"
#include "../Utils/Enums.h"

using namespace std;

//Forward declarations
class Scene;

class Game {
public:
    sf::RenderWindow window;
    DJ dj;
    bool paused = false;
    bool game_over = false;
    uint level = 0;
    bool fullscreen = false;
    sf::Font default_font;

    Game(const char* title, uint win_w, uint win_h, uint init_fps, bool fullscrn = false);
    
    //Game stuff
    void HandleEvents();
    void ProcessInput();
    void Update();
    void Render();

    //Getters
    uint GetFPS() const { return fps; }
    int GetDebugTimer() const { return debug_timer; }
    uint GetFramesElapsed() const { return frames_elapsed; }
    float GetMusicVolume() const { return music_volume; }
    float GetSFXVolume() const { return sfx_volume; }
    sf::Vector2u GetResolution() const { return resolution; }
    float GetResScale() const { return resolution.x / MINRESW; }

    //Setters
    void SetScene(Scenes scn);
    void SetMusicVolume(float n_v);
    void SetSFXVolume(float n_v);
    void SetResolution(float res_scalar);
    void SetResolution(sf::Vector2u n_r);

    //Scenes
    unordered_map<Scenes, shared_ptr<Scene>> scenes;
    weak_ptr<Scene> active_scene;
    weak_ptr<Scene> old_scene;
    shared_ptr<Scene> title_scene;
    shared_ptr<Scene> game_scene;

private:
    //Variables
    uint fps = 0;
    int debug_timer = 0;
    uint frames_elapsed = 0;
    float music_volume = 100;
    float sfx_volume = 100;
    sf::Vector2u resolution;
};