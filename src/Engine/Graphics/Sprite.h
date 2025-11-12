#pragma once
#include <string>
#include <SDL3_image/SDL_image.h>
#include "../Math/Geometry.h" //(Vec2 (iostream))

class Renderer;

class Sprite {
    friend class Renderer;
public:
    struct Info {
        std::string sheet;
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
        bool ping_pong = false; //Whether the animation plays in reverse upon finishing
        uchar game_frames = 0; //How many game frames have elapsed since the last frame change
        uchar fci = 0; //frame_change_interval: How many game frames until the next animation frame
        float anim_length = 0.f; //Length of the animation in seconds
        int dfc = 0; //Distance from camera; draw order, basically - the lower the number, the closer to the camera
        Info() = default;
    };

    //ANY SPRITE (OR SOMETHING THAT HAS ONE) THAT IS COPY/MOVE CONSTRUCTED MUST BE INITIALIZED ON THE HEAP - JUST. FUCKING. TRUST!

    Sprite(const Info& i) : info(i) { Init(i); }
    Sprite() { Init(info); }
    ~Sprite() {
        if (texture) {
            SDL_DestroyTexture(texture);
            texture = nullptr;
        }
    }
    void Init(const Info& i);

    static inline void SetSDLRenderer(SDL_Renderer* r) { sdl_renderer = r; }
    static inline void SetRenderer(Renderer* r) { renderer = r; }

    static inline void SetGameFPS(const float gfps) { game_fps = gfps; }

    [[nodiscard]] inline SDL_Texture* GetTexture() const { return texture; }

    void Update();
    void Draw() const;

    void SetSheet(const std::string& new_sheet);
    [[nodiscard]] inline std::string GetSheet() const { return info.sheet; }
    [[nodiscard]] inline Vec2i GetSheetSize() const { return info.sheet_size; }

    template<typename T>
    inline void MoveTo(const Vec2<T>& new_pos) { info.pos = Round(new_pos); }
    template<typename T>
    inline void MoveBy(const Vec2<T>& offset) { info.pos += Round(offset); }
    [[nodiscard]] inline Vec2i GetPos() const { return info.pos; }

    inline void SetSize(const Vec2i& s) { info.spr_size = s; }
    [[nodiscard]] inline Vec2i GetSprSize() const { return info.spr_size; }
    [[nodiscard]] inline Vec2i GetFrameSize() const { return info.frame_size; }
    [[nodiscard]] inline Vec2i GetScaledSize() const { return { info.spr_size.x * info.scale.x, info.spr_size.y * info.scale.y }; }

    inline void SetScale(const Vec2i& s) { info.scale = s; }
    [[nodiscard]] inline Vec2i GetScale() const { return info.scale; }

    //Rotation
    inline void SetRot(const float angle, const bool rad = false) { info.rot = angle * (57.2958f * rad); }
    inline void AlterRot(const float offset, const bool rad = false) { info.rot += offset * (57.2958f * rad); }
    [[nodiscard]] inline float GetRot(const bool rad = false) const { return rad ? info.rot : info.rot / 57.2958f; }

    inline void SetColor(const Color& c) { info.tint = c; }
    [[nodiscard]] inline Color GetColor() const { return info.tint; }

    void SetSheetRow(uchar new_s_r, const uchar new_n_f = 0);
    [[nodiscard]] inline uchar GetSheetRow() const { return info.sheet_row; }

    void SetCurrFrame(uchar new_c_f);
    [[nodiscard]] inline uchar GetCurrFrame() const { return info.curr_frame; }

    inline void SetNumFrames(const uchar new_n_f) { info.num_frames = new_n_f; }
    [[nodiscard]] inline uchar GetNumFrames() const { return info.num_frames; }

    void SetAnimFPS(const char new_fps);
    [[nodiscard]] inline char GetAnimFPS() const { return info.anim_fps; }

    void SetPingPong(const bool pp = true) { info.ping_pong = pp; }
    [[nodiscard]] inline bool GetPingPong() const { return info.ping_pong; }

    void SetOrigin(const Vec2f new_ori = { .5f, .5f });
    [[nodiscard]] inline Vec2f GetOrigin() const { return info.origin; }; //Returns origin from 0-1
    [[nodiscard]] inline Vec2f GetOriginSprite() const { return { info.origin.x * info.spr_size.x, info.origin.y * info.spr_size.y }; }; //Returns origin in pixels relative to the sprite size
    [[nodiscard]] inline Vec2f GetOriginFrame() const { return { info.origin.x * info.frame_size.x, info.origin.y * info.frame_size.y }; }; //Returns origin in pixels relative to the frame size

    inline void SetDFC(const int new_dfc = 0) { info.dfc = new_dfc; }
    [[nodiscard]] inline int GetDFC() const { return info.dfc; }

private:
    Info info = {}; //private because whenever a member is set, other ancillary functions must be performed
    SDL_Texture* texture = nullptr;
    static inline SDL_Renderer* sdl_renderer;
    static inline Renderer* renderer;
    static inline float game_fps = 60;
};