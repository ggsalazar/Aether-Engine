#include <thread>
#include <string>
#include "Game.h"
#include "../Core/Input.h"
#include "../Core/Math.h"

Game::Game(const char* title, const uint init_fps)
    : fps(init_fps), resolution(min_res * 2), camera({ 0 }, Vec2i(min_res)),
    window("Game", resolution), renderer(window.GetWin(), &camera) {

    //Set random seed
    srand((uint)time(nullptr));

    //Initialize SDL
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS);
    TTF_Init();

    //Delta time
    target_frame_time = 1.f / fps;
    last_time = hr_clock::now();

    //Set the resolution, camera dimensions, and tile size
    resolution = window.WinSize();
    SetResolution(resolution);
    Sprite::SetRenderer(renderer.GetRenderer());

    //Set the Text camera
    Text::SetCam(&camera);


    //Initialize the Input namespace
    Input::Init(&window, &camera);

    //Initialize fonts
    for (int i = 12; i < 301; i += 2)
        default_fonts.insert({ i, Font("m5x7", i) });
    debug_txt.Init(&default_fonts[36]);

    //Initialize the DJ's tracks
    //Play the title track - TO-DO

    //Init scene, which sets the Game* in all the classes that need it
    //gm.Init(this);
    //Open the title scene
    //gm.ChangeScene();

    //Initialize cursor
    //Cursor sprite info
    //Sprite::Info csi = {};
    //csi.sheet = "UI/Cursors"; csi.frame_size = csi.spr_size = { 16 };
    //cursor.Init(csi);
    //SDL_SetWindowRelativeMouseMode(); //This will lock the cursor to the game window
    //SDL_HideCursor();
}

void Game::Run() {
    //Calculate delta time
    auto now = hr_clock::now();
    delta = now - last_time;
    last_time = now;
    delta_time = delta.count();

    //Handle events
    window.PollEvents();

    //Process input and update the game state once every 60th of a second
    accumulated_time += delta_time;
    if (accumulated_time >= target_frame_time) {
        accumulated_time -= target_frame_time;
        if (++game_frames >= fps) game_frames = 0;

        //Process input
        ProcessInput();
        //Update the game world
        Update();
    }


    //Draw the game world
    if (window.open) Render();
    else running = false;

    //Framerate cap
    auto frame_time = (hr_clock::now() - now).count();
    if (frame_time < target_frame_time)
        this_thread::sleep_for(durationf(target_frame_time - frame_time));
}

//Process input
void Game::ProcessInput() {

    //Update cursor position
    cursor.MoveTo(Input::MousePos());
}

//Update the game world
void Game::Update() {
    //Reset our input variables
    Input::Update();

    //Update the current scene

}

//Draw the game world
void Game::Render() {

    renderer.BeginFrame(); //This also clears the frame


    renderer.DrawSprite(cursor);

    renderer.EndFrame();
}

void Game::SetMusicVolume(float n_v) {
    Math::Clamp(n_v, 0, 100);
    music_volume = n_v;
    //dj.SetVolume(music_volume);
}

void Game::SetSFXVolume(float n_v) {
    Math::Clamp(n_v, 0, 100);
    sfx_volume = n_v;
}

void Game::SetResolution(uchar res_scalar) {
    //Minimum resolution is 640 x 360
    if (res_scalar > 0) {
        Vec2u new_win_size = { res_scalar * min_res.x, res_scalar * min_res.y };
        while (new_win_size.x > window.ScreenSize().x or new_win_size.y > window.ScreenSize().y) {
            --res_scalar;
            new_win_size = { res_scalar * min_res.x, res_scalar * min_res.y };
        }
        resolution = new_win_size;
    }
    else {
        res_scalar = 1;
        resolution = min_res;
    }

    SetRes();
}

void Game::SetResolution(Vec2u n_r) {

    if (n_r.x > 0 and n_r.y > 0) {
        n_r.x = n_r.x <= window.ScreenSize().x ? n_r.x : window.ScreenSize().x;
        n_r.y = n_r.y <= window.ScreenSize().y ? n_r.y : window.ScreenSize().y;

        resolution = n_r;

        SetRes();
    }

}

void Game::SetRes() {
    //Resize the window
    if (resolution == window.ScreenSize())
        SDL_SetWindowFullscreen(window.GetWin(), true);
    else {
        SDL_SetWindowFullscreen(window.GetWin(), false);
        SDL_SetWindowSize(window.GetWin(), resolution.x, resolution.y);

        //Move the window
        SDL_Rect screen_bounds;
        SDL_DisplayID dID = SDL_GetDisplayForWindow(window.GetWin());
        SDL_GetDisplayBounds(dID, &screen_bounds);
        SDL_SetWindowPosition(window.GetWin(), (int)(screen_bounds.w * .5f - resolution.x * .5f), (int)(screen_bounds.h * .5f - resolution.y * .5f));
    }

    //Set the renderer's window size
    renderer.SetWinSize();

    //Set the res_scale for Text
    Text::SetResScale(resolution.x / min_res.x);
}
