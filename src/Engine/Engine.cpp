#include <thread>
#include "Engine.h"
#include "Input.h"
#include "Graphics/Text.h"
#include "Math/Math.h"

Engine::Engine(const char* title, const float init_fps)
    : fps(init_fps), resolution(min_res*2), window(title, resolution),
    renderer(window.GetWin(), &camera), camera({ 0 }, Vec2i(min_res)) {

    //Set random seed
    srand((uint)time(nullptr));

    //Initialize SDL
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    TTF_Init();

    //Delta time
    target_frame_time = 1.f / fps;
    last_time = hr_clock::now();

    //Set the resolution
    resolution = window.WinSize();
    //Initialize the Input namespace
    Input::Init(&window, &camera);
    SetResolution(resolution);

    //Set sprite's renderer & game fps
    Sprite::SetSDLRenderer(renderer.GetRenderer());
    Sprite::SetRenderer(&renderer);
    Sprite::SetGameFPS(fps);

    //Initialize text fonts
    Text::InitFonts();

    //Init game, which sets the Engine* in all the classes that need it
    game.Init(this);
    game.ChangeScene(Scene::Title);

    //SDL_SetWindowRelativeMouseMode(); //This will lock the cursor to the game window
    SDL_HideCursor();
}

void Engine::Run() {
    //Calculate delta time
    auto now = hr_clock::now();
    delta_time = (now - last_time).count();
    accumulated_time += delta_time;
    last_time = now;

    //Handle events
    window.PollEvents();

    //Process input and update the game state once every 60th of a second
    if (accumulated_time >= target_frame_time) {
        accumulated_time -= target_frame_time;
        if (++game_frames >= fps) game_frames = 0;

        //Update the game
        game.Update();

        //Reset the input arrays - must come *after* querying input from the player
        Input::Update();
    }

    //Draw the game world
    if (window.open) Render();
    else running = false;
}

//Draw the game world
void Engine::Render() {
    renderer.BeginFrame(); //This also clears the frame

    game.Draw();
    game.DrawGUI();

    renderer.EndFrame();
}

void Engine::SetSFXVolume(float n_v) {
    Math::Clamp(n_v, 0, 100);
    sfx_volume = n_v;
}

void Engine::SetResolution(uchar res_scalar) {
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

void Engine::SetResolution(Vec2u n_r) {
    if (n_r.x > 0 and n_r.y > 0) {
        n_r.x = n_r.x <= window.ScreenSize().x ? n_r.x : window.ScreenSize().x;
        n_r.y = n_r.y <= window.ScreenSize().y ? n_r.y : window.ScreenSize().y;

        resolution = n_r;

        SetRes();
    }
}

void Engine::SetRes() {
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

    //Update the input namespace's resolution
    Input::UpdateRes();

    //Set the res_scale for Text
    Text::SetResScale(resolution.x / min_res.x);
}