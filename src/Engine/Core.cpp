#include <thread>
#include "Core.h"
#include "Input.h"
#include "Graphics/Text.h"
#include "Math/Math.h"

Core::Core(const char* title, const float init_fps)
    : fps(init_fps), resolution(min_res * 2), window(title, resolution),
    renderer(window.GetWin()) {

    //Set random seed
    srand((uint)time(nullptr));

    //Initialize SDL
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    TTF_Init();

    //Delta time
    target_frame_time = durationf{1.f / fps};
    last_time = hr_clock::now();

    //Initialize the Input namespace
    Input::Init(&window);

    //Set Sprite's renderer & game fps
    TextureManager::SetSDLRenderer(renderer.GetRenderer());
    Sprite::SetRenderer(&renderer);
    Sprite::SetTexMan(&tex_man);
    Sprite::SetGameFPS(fps);

    //Init Text fonts & renderer
    Text::SetRenderer(&renderer);
    Text::InitFonts();

    //Init gm, which sets the Game* in all the classes that need it
    game.Init(this);

    //Set the camera for the renderer and the Input namespace
    renderer.SetCamera(&game.camera);
    Input::SetCamera(&game.camera);

    //Set the resolution (also sets Input's res_scale)
    SetResolution(resolution);

    //Open the title scene
    game.ChangeScene(Scene::Title);

    //SDL_SetWindowRelativeMouseMode(); //This will lock the cursor to the game window
    SDL_HideCursor();
}

void Core::Run() {
    //Calculate delta time
    now = hr_clock::now();
    delta_time = now - last_time;
    accumulated_time += delta_time;
    last_time = now;

    //Handle events
    window.PollEvents();

    //Process input and update the game state once every 60th of a second
    while (accumulated_time >= target_frame_time) {
        accumulated_time -= target_frame_time;

        if (accumulated_time < target_frame_time) {
            if (++game_frames >= fps) game_frames = 0;

            game.Update();
            //Reset input arrays - must be done after querying input
            Input::Update();
        }
    }

    //Draw the game world
    if (window.open) Render();
    else running = false;

    //Frame limiter
    frame_time = hr_clock::now() - now;
    if (frame_time < target_frame_time)
        this_thread::sleep_for(target_frame_time - frame_time);
}

//Draw the game world
void Core::Render() {
    renderer.BeginFrame();

    game.Draw();
    game.DrawGUI();

    renderer.Render();

    renderer.EndFrame();
}

void Core::SetSFXVolume(float n_v) {
    Math::Clamp(n_v, 0, 200);
    sfx_volume = n_v;
}

void Core::SetResolution(uchar res_scalar) {
    //Minimum resolution is 640 x 360
    if (res_scalar > 0) {
        Vec2u new_win_size = { res_scalar * min_res.x, res_scalar * min_res.y };
        while (new_win_size.x > window.GetScreenSize().x or new_win_size.y > window.GetScreenSize().y) {
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

void Core::SetResolution(Vec2u n_r) {

    if (n_r.x > 0 and n_r.y > 0) {
        n_r.x = n_r.x <= window.GetScreenSize().x ? n_r.x : window.GetScreenSize().x;
        n_r.y = n_r.y <= window.GetScreenSize().y ? n_r.y : window.GetScreenSize().y;

        resolution = n_r;

        SetRes();
    }

}

void Core::SetRes() {
    //Resize the window
    if (resolution == window.GetScreenSize())
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

    //Update Input's resolution
    Input::UpdateResScale(resolution.x);
}