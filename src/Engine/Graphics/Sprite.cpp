#include "Sprite.h"
#include <filesystem>
#include "Renderer.h"

namespace fs = std::filesystem;

void Sprite::Init(const Info& i) {
    info = i;

    SetSheet(info.sheet);
}

void Sprite::Update() {
    if (info.anim_fps != 0 and ++info.game_frames >= info.fci) {
        //Game frames set in SetFrame
        if (info.anim_fps > 0) SetFrame(++info.frame);
        else if (info.anim_fps < 0) SetFrame(--info.frame);
    }
}

void Sprite::Draw(LayerName l) const {
    l = l == LayerName::NONE ? info.default_layer : l;
    renderer->SubmitSprite(l, *this);
}

void Sprite::SwitchAnim(const Anim new_anim) {

}

void Sprite::SetSheet(const string &new_sheet) {
    info.sheet = new_sheet;

    if (info.sheet.size()) {
        string path_name, sprite_name, file_name, sheet_png;
        size_t pos = 0;
        //This should NEVER return npos since EVERY sprite should be in a directory
        pos = info.sheet.find_last_of('/');
        path_name = info.sheet.substr(0, pos+1);
        sprite_name = info.sheet.substr(pos+1);

        for (const auto& file : fs::directory_iterator("../assets/Sprites/" + path_name)) {
            if (!file.is_regular_file()) continue;

            file_name = file.path().filename().string();
            //We've found our file! Assign it to sheet_png to be passed to the texture manager
            if (file_name.find(sprite_name) != string::npos)
                sheet_png = path_name + file_name;
        }

        if (sheet_png.size())
            texture = tex_man->LoadTexture(sheet_png);
        else {
            std::cerr << "Sprite::Init(): Could not find the sheet for current Sprite!\n";
            return;
        }
        SetAnimFPS(info.anim_fps);

        SDL_GetTextureSize(texture, &info.sheet_size.x, &info.sheet_size.y);

        //Do we know the number of frames?
        if (info.num_frames == 1) {
            string num_frms = sheet_png;
            size_t pos = num_frms.rfind('_');
            //Assume the character after the last _ is a number (will throw errors if this is not the case!)
            if (pos != string::npos)
                info.num_frames = stoi(num_frms.substr(pos+1));
        }
        //Calculate frame size
        info.frame_size = Vec2{info.sheet_size.x/info.num_frames, info.sheet_size.y};

        //Sprite size
        info.spr_size = info.frame_size * info.scale;
    }

}

void Sprite::SetFrame(uchar new_f) {
    if (info.anim_fps > 0) {
        if (info.ping_pong and new_f == info.num_frames-1) info.anim_fps *= -1;
        else
            while (new_f >= info.num_frames) new_f -= info.num_frames;
    }

    else if (info.anim_fps < 0 and info.ping_pong and new_f == 0)
        info.anim_fps *= -1;

    info.frame = new_f;
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