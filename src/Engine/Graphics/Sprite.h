#pragma once
#include <string>
#include <SDL3_image/SDL_image.h>
#include "../Math/Geometry.h" //(Vec2 (iostream))

class Sprite {
    friend class Renderer;
public:
    struct Info {
        std::string sheet = "";
        Vec2i pos; //Worldspace position of the sprite
        Vec2f origin; //Anchor point of the sprite
        Vec2i spr_size; //The size of the sprite in world space
        Vec2i frame_size; //The size of a single frame
        Vec2i sheet_size; //The size of the sprite's spritesheet
        Vec2i scale{ 1 }; //The scale of the sprite
        Color tint{ 1 }; //The color tint of the sprite
        float rot = 0.f; //Angle of rotation in degrees
        uchar sheet_row = 0; //Which row of the sheet our current animation is on (each row should be a different animation)
        uchar num_frames = 1; //How many frames are in the CURRENT sheet row
        uchar curr_frame = 0; //Which frame of the animation we are currently on
        char anim_fps = 0; //How many frames of the animation play per second
        uchar game_frames = 0; //How many game frames have elapsed since the last frame change
        uchar fci = 0; //frame_change_interval: How many game frames until the next animation frame
        float anim_length = 0.f; //Length of the animation in seconds
        int dfc = 0; //Distance from camera; draw order, basically - the lower the number, the closer to the camera
    };

    Sprite(const Info& i = {}) : info(i) { Init(i); }
    ~Sprite() {
        if (texture) {
            SDL_DestroyTexture(texture);
            texture = nullptr;
        }
    }
    void Init(const Info& i);

    static inline void SetRenderer(SDL_Renderer* r) { renderer = r; }

    inline SDL_Texture* GetTexture() const { return texture; }

    void Update();

    void SetSheet(std::string new_sheet);
    inline std::string GetSheet() const { return info.sheet; }
    inline Vec2i GetSheetSize() const { return info.sheet_size; }

    inline Vec2i GetPos() const { return info.pos; }
    inline void MoveTo(const Vec2i& new_pos) { info.pos = new_pos; }
    inline void MoveBy(const Vec2i& offset) { info.pos += offset; }

    inline void SetSize(const Vec2i& s) { info.spr_size = s; }
    inline Vec2i GetSprSize() const { return info.spr_size; }
    inline Vec2i GetFrameSize() const { return info.frame_size; }
    inline Vec2i GetScaledSize() const { return { info.spr_size.x * info.scale.x, info.spr_size.y * info.scale.y }; }

    inline void SetScale(const Vec2i& s) { info.scale = s; }
    inline Vec2i GetScale() const { return info.scale; }

    //Rotation in degrees
    inline void SetRotD(const float angle) { info.rot = angle; }
    inline float GetRotD() const { return info.rot; }
    //Rotation in radians
    inline void SetRotR(const float rad) { info.rot = rad * 57.2958; }
    inline float GetRotR() const { return info.rot / 57.2958; }

    inline void SetColor(const Color& c) { info.tint = c; }
    inline Color GetColor() const { return info.tint; }

    void SetSheetRow(uint new_s_r, const uint new_n_f = 0);
    inline uint GetSheetRow() const { return info.sheet_row; }

    void SetCurrFrame(uint new_c_f);
    inline uint GetCurrFrame() const { return info.curr_frame; }

    inline void SetNumFrames(const uint new_n_f) { info.num_frames = new_n_f; }
    inline uint GetNumFrames() const { return info.num_frames; }

    void SetAnimFPS(const int new_fps);
    inline uint GetAnimFPS() const { return info.anim_fps; }

    void SetOrigin(const Vec2f new_ori = { .5f, .5f });
    inline Vec2f GetOrigin() const { return info.origin; }; //Returns origin from 0-1
    inline Vec2f GetOriginSprite() const { return { info.origin.x * info.spr_size.x, info.origin.y * info.spr_size.y }; }; //Returns origin in pixels relative to the sprite size
    inline Vec2f GetOriginFrame() const { return { info.origin.x * info.frame_size.x, info.origin.y * info.frame_size.y }; }; //Returns origin in pixels relative to the frame size

    inline void SetDFC(const int new_dfc = 0) { info.dfc = new_dfc; }
    inline int GetDFC() const { return info.dfc; }

private:
    Info info; //private because whenever a member is set, other ancillary functions must be performed
    SDL_Texture* texture = nullptr;
    inline static SDL_Renderer* renderer;
    inline static uint game_fps = 60;
};