#include <SDL3/SDL_render.h>
#include "Sprite.h"
#include "Renderer.h"

void Sprite::Init(const Info& i) {
    info = i;

    if (!info.sheet.empty()) {

        SetAnimFPS(info.anim_fps);
        std::string sheet_png = "../assets/Sprites/" + info.sheet + ".png";
        if (texture) {
            SDL_DestroyTexture(texture);
            texture = nullptr;
        }
        texture = IMG_LoadTexture(sdl_renderer, sheet_png.c_str());

        if (!texture)
            std::cout << "Could not load texture from file: " << sheet_png << "!\n";

        SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);

        Vec2f s_size;
        SDL_GetTextureSize(texture, &s_size.x, &s_size.y);
        info.sheet_size = Round(s_size.x, s_size.y);

        info.frame_size = info.frame_size == Vec2i{0} ? info.sheet_size : info.frame_size;
        info.spr_size = info.spr_size == Vec2i{0} ? info.frame_size : info.spr_size;

        //Automatically set num_frames if it wasn't already set
        if (info.num_frames == 1) info.num_frames = info.sheet_size.x / info.frame_size.x;
    }
}

void Sprite::Update() {
    if (info.anim_fps != 0 and ++info.game_frames >= info.fci) {

        //Game frames set in SetCurrFrame
        if (info.anim_fps > 0) SetCurrFrame(++info.curr_frame);
        else if (info.anim_fps < 0) SetCurrFrame(--info.curr_frame);
    }
}

void Sprite::Draw() const {
    renderer->DrawSprite(*this);
}

void Sprite::SetSheet(const std::string &new_sheet) {
    info.sheet = new_sheet;

    if (!info.sheet.empty()) {

        std::string sheet_png = "../assets/Sprites/" + info.sheet + ".png";
        if (texture) {
            SDL_DestroyTexture(texture);
            texture = nullptr;
        }
        texture = IMG_LoadTexture(sdl_renderer, sheet_png.c_str());

        if (!texture)
            std::cout << "Could not load texture from file: " << sheet_png << "!\n";

        SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);

        Vec2f s_size;
        SDL_GetTextureSize(texture, &s_size.x, &s_size.y);
        info.sheet_size = Round(s_size.x, s_size.y);
    }
}

void Sprite::SetSheetRow(uchar new_s_r, const uchar new_n_f) {
    //Dividing the height of the sheet by the height of the frame should ALWAYS produce a whole number
    const uchar num_rows = info.sheet_size.y / info.frame_size.y;
    while (0 > new_s_r or new_s_r >= num_rows) {
        if (new_s_r < 0) new_s_r += num_rows;
        else if (new_s_r >= num_rows) new_s_r -= num_rows;
    }
    info.sheet_row = new_s_r;

    //If a new number of frames is not provided, it will be assumed that the # of frames is staying the same
    if (new_n_f != 0)
        SetNumFrames(new_n_f);
}

void Sprite::SetCurrFrame(uchar new_c_f) {
    if (info.anim_fps > 0) {
        if (info.ping_pong and new_c_f == info.num_frames-1) {
            info.anim_fps *= -1;
        }
        else
            while (new_c_f >= info.num_frames) new_c_f -= info.num_frames;
    }

    else if (info.anim_fps < 0) {
        if (info.ping_pong and new_c_f == 0) {
            info.anim_fps *= -1;
        }
    }

    info.curr_frame = new_c_f;
}

void Sprite::SetAnimFPS(const char new_fps) {
    info.anim_fps = new_fps;

    if (info.anim_fps != 0) {
        info.fci = abs(round(game_fps / info.anim_fps));

        info.anim_length = (float)info.num_frames / (float)info.anim_fps;
    }
    else {
        info.fci = 0;
        info.anim_length = 0;
    }
}

void Sprite::SetOrigin(const Vec2f new_ori) {
    info.origin.x = (new_ori.x < 0.f or 1.f < new_ori.x) ? .5 : new_ori.x;
    info.origin.y = (new_ori.y < 0.f or 1.f < new_ori.y) ? .5 : new_ori.y;
}