#pragma once
#include "TextureManager.h" //string, unordered_map, SDL_image
#include "../Math/Geometry.h" //(Vec2 (iostream))
#include "src/Engine/Enums.h"

using std::string;

class Renderer;

class Sprite {
    friend class Renderer;
public:
    struct Info {
        Anim anim = Anim::NONE;
        LayerName default_layer = LayerName::NONE;
        string sheet;
        Vec2i pos; //Worldspace position of the sprite
        Vec2f origin; //Anchor point of the sprite
        Vec2i spr_size; //The size of the sprite in world space
        Vec2i frame_size; //The size of a single frame
        Vec2f sheet_size; //The size of the sprite's spritesheet
        Vec2i scale{ 1 }; //The scale of the sprite
        Color tint{ 1 }; //The color tint of the sprite
        float rot = 0.f; //Angle of rotation in degrees
        uchar num_frames = 1; //How many frames are in the animation
        uchar frame = 0; //Which frame of the animation we are currently on
        char anim_fps = 0; //How many frames of the animation play per second
        bool ping_pong = false; //Whether the animation plays in reverse upon finishing
        uchar game_frames = 0; //How many game frames have elapsed since the last frame change
        uchar fci = 0; //frame_change_interval: How many game frames until the next animation frame
        float anim_length = 0.f; //Length of the animation in seconds
        int layer_order = 0; //When it gets drawn relative to other things in the same layer
        Info() = default;
    };

    Sprite(const Info& i) : info(i) { Init(i); }
    Sprite() { Init(info); }
    ~Sprite() { texture = nullptr; }
    void Init(const Info& i);

    static inline void SetRenderer(Renderer* r) { renderer = r; }
    static inline void SetTexMan(TextureManager* t) { tex_man = t; }
    static inline void SetGameFPS(const float gfps) { game_fps = gfps; }

    [[nodiscard]] inline SDL_Texture* GetTexture() const { return texture; }

    void Update();
    void Draw(LayerName l = LayerName::NONE) const;

    void SwitchAnim(const Anim new_anim);
    void SetSheet(const string& new_sheet);
    [[nodiscard]] inline string GetSheet() const { return info.sheet; }
    [[nodiscard]] inline Vec2i GetSheetSize() const { return info.sheet_size; }

    void SetDefaultLayer(const LayerName layer) { info.default_layer = layer; }
    [[nodiscard]] inline LayerName GetDefaultLayer() const { return info.default_layer; }

    inline void SetLayerOrder(const int new_lo = 0) { info.layer_order = new_lo; }
    [[nodiscard]] inline int GetLayerOrder() const { return info.layer_order; }

    template<typename T=float>
    inline void MoveTo(const Vec2<T> new_pos) { info.pos = Round(new_pos); }
    template<typename T=float>
    inline void MoveBy(const Vec2<T> offset) { info.pos += Round(offset); }
    [[nodiscard]] inline Vec2i GetPos() const { return info.pos; }

    inline void SetSize(const Vec2i& s) { info.spr_size = s; info.scale = info.spr_size/info.frame_size; }
    [[nodiscard]] inline Vec2i GetSprSize() const { return info.spr_size; }
    [[nodiscard]] inline Vec2i GetFrameSize() const { return info.frame_size; }

    inline void SetScale(const Vec2i s) { info.scale = s; info.spr_size = info.scale * info.frame_size; }
    [[nodiscard]] inline Vec2i GetScale() const { return info.scale; }

    //Rotation
    inline void SetRot(const float angle, const bool rad = false) { info.rot = angle * (57.2958f * rad); }
    inline void AlterRot(const float offset, const bool rad = false) { info.rot += offset * (57.2958f * rad); }
    [[nodiscard]] inline float GetRot(const bool rad = false) const { return rad ? info.rot : info.rot / 57.2958f; }

    inline void SetColor(const Color& c) { info.tint = c; }
    [[nodiscard]] inline Color GetColor() const { return info.tint; }

    void SetFrame(uchar new_f);
    [[nodiscard]] inline uchar GetFrame() const { return info.frame; }

    inline void SetNumFrames(const uchar new_n_f) { info.num_frames = new_n_f; }
    [[nodiscard]] inline uint GetNumFrames() const { return info.num_frames; }

    void SetAnimFPS(const char new_fps);
    [[nodiscard]] inline uint GetAnimFPS() const { return info.anim_fps; }

    void SetPingPong(const bool pp = true) { info.ping_pong = pp; }
    [[nodiscard]] inline bool GetPingPong() const { return info.ping_pong; }

    void SetOrigin(const Vec2f new_ori = { .5f, .5f });
    [[nodiscard]] inline Vec2f GetOrigin() const { return info.origin; }; //Returns origin from 0-1
    [[nodiscard]] inline Vec2f GetOriginSprite() const { return { info.origin.x * info.spr_size.x, info.origin.y * info.spr_size.y }; }; //Returns origin in pixels relative to the sprite size
    [[nodiscard]] inline Vec2f GetOriginFrame() const { return { info.origin.x * info.frame_size.x, info.origin.y * info.frame_size.y }; }; //Returns origin in pixels relative to the frame size

private:
    Info info = {}; //private because whenever a member is set, other ancillary functions are sometimes performed
    static inline TextureManager* tex_man;
    static inline Renderer* renderer;
    SDL_Texture* texture = nullptr;
    static inline float game_fps = 60.f;
};