#include "Sprite.h"

void Sprite::Init(const Info& i) {
    info = i;

    if (info.sheet != "") {

        SetAnimFPS(info.anim_fps);

        std::string sheet_png = "assets/Sprites/" + info.sheet + ".png";
        if (texture) {
            SDL_DestroyTexture(texture);
            texture = nullptr;
        }
        texture = IMG_LoadTexture(renderer, sheet_png.c_str());

        if (!texture)
            std::cout << "Could not load texture from file: " << sheet_png << "!\n";

        SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);

        Vec2f s_size;
        SDL_GetTextureSize(texture, &s_size.x, &s_size.y);
        info.sheet_size = Round(s_size.x, s_size.y);
        if (info.frame_size == Vec2i{ 0 }) info.frame_size = info.sheet_size;
        if (info.spr_size == Vec2i{ 0 }) info.spr_size = info.frame_size;
    }
}

void Sprite::Update() {
    if (info.anim_fps != 0 and ++info.game_frames >= info.fci) {
        info.game_frames = 0;

        if (info.anim_fps > 0) SetCurrFrame(++info.curr_frame);
        else if (info.anim_fps < 0) SetCurrFrame(--info.curr_frame);
    }
}

void Sprite::SetSheet(std::string new_sheet) {
    new_sheet = "assets/Sprites/" + new_sheet + ".png";
    if (texture) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
    texture = IMG_LoadTexture(renderer, new_sheet.c_str());

    if (!texture)
        std::cout << "Could not load texture from file: " << new_sheet << "!\n";

    SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);

    Vec2f s_size;
    SDL_GetTextureSize(texture, &s_size.x, &s_size.y);
    info.sheet_size = Round(s_size.x, s_size.y);
    if (info.frame_size == Vec2i{ 0 }) info.frame_size = info.sheet_size;
    if (info.spr_size == Vec2i{ 0 }) info.spr_size = info.frame_size;
}

void Sprite::SetSheetRow(uint new_s_r, const uint new_n_f) {
    //Dividing the height of the sheet by the height of the frame should ALWAYS produce a whole number
    const uint num_rows = info.sheet_size.y / info.frame_size.y;
    while (0 > new_s_r or new_s_r >= num_rows) {
        if (new_s_r < 0) new_s_r += num_rows;
        else if (new_s_r >= num_rows) new_s_r -= num_rows;
    }
    info.sheet_row = new_s_r;

    //If a new number of frames is not provided, it will be assumed that the # of frames is staying the same
    if (new_n_f != 0)
        SetNumFrames(new_n_f);
}

void Sprite::SetCurrFrame(uint new_c_f) {
    if (info.anim_fps > 0)
        while (new_c_f >= info.num_frames) new_c_f -= info.num_frames;

    else if (info.anim_fps < 0)
        while (new_c_f < 0) new_c_f += info.num_frames;

    info.curr_frame = new_c_f;
}

void Sprite::SetAnimFPS(const int new_fps) {
    info.anim_fps = new_fps;

    if (info.anim_fps != 0) {
        info.fci = abs(round(game_fps / info.anim_fps));

        info.anim_length = info.num_frames / info.anim_fps;
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