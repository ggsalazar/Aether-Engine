#pragma once
#include "../Core/Collision.h" //Geometry (Vec2 (iostream))
#include "../Game/Game.h"
#include "../Game/Scene.h"
#include "../Graphics/Sprite.h"
#include "../Graphics/Text.h"

class Entity {
public:
    //Variables
    Vec2i size = { 0, 0 };
    Sprite sprite;

    //SFX Stuff
    //sf::SoundBuffer sb;
    //sf::Sound sound;

    Entity(const Sprite::Info s_i);
    virtual ~Entity() = default;

    static inline void SetGame(Game* g) { game = g; }

    inline virtual void GetInput() {}
    inline virtual void Update() { sprite.SetDFC(-pos.y); }
    virtual void Draw();
    inline virtual void DrawGUI() {}


    virtual void MoveBy(const Vec2f offset);
    virtual void MoveTo(const Vec2f new_pos);

    inline Vec2i GetPos() const { return pos; }
    inline Rect GetBBox() const { return bbox; }

    void PlaySound();

protected:
    Vec2i pos;
    Rect bbox;

    inline static Game* game = nullptr;
    static int SEC;


    virtual void Move();
};