#pragma once
#include "../Engine/Collision.h" //Geometry (Vec2 (iostream))
#include "../Engine/Engine.h" //Camera, Renderer, Game (Enums, Sprite)

class Entity {
public:
    //Variables
    Vec2i size = { 0, 0 };
    Sprite sprite;
    inline static uchar SEC;
    bool expired = false;

    //SFX Stuff
    //sf::SoundBuffer sb;
    //sf::Sound sound;

    Entity(const Sprite::Info s_i);
    virtual ~Entity() = default;

    static void SetEngine(Engine* e, Game* g);

    inline virtual void GetInput() {}
    inline virtual void Update() { sprite.Update(); sprite.SetDFC(-pos.y); }
    virtual void Draw();
    inline virtual void DrawGUI() {}


    virtual void MoveBy(const Vec2f offset);
    virtual void MoveBy(const Vec2i offset);
    virtual void MoveTo(const Vec2f new_pos);
    virtual void MoveTo(const Vec2i new_pos);

    [[nodiscard]] inline Vec2i GetPos() const { return pos; }
    [[nodiscard]] inline Rect GetBBox() const { return bbox; }

    void PlaySFX();
    void PlaySFXPitchShift();

protected:
    Vec2i pos;
    Rect bbox;

    static Engine* engine;
    static Game* game;

    virtual void Move();
};