#pragma once
#include "../Engine/Core.h" //Camera, Renderer, Game (Enums, Sprite)
#include "../Engine/Math/Geometry.h" //Vec2 (iostream)

class Entity {
public:
    Vec2i size;
    inline static float SEC;
    bool expired = false;
    Sprite sprite;

    explicit Entity(const Sprite::Info& spr_info = {});
    virtual ~Entity() = default;

    static void SetCore(Core* c, Game* g);

    inline virtual void Update() { sprite.Update(); sprite.SetLayerOrder(pos.y); }
    inline virtual void Draw() {}
    inline virtual void DrawGUI() {}

    inline virtual void MoveBy(const Vec2f offset) {
        pos += offset;
        Entity::Move();
    }
    inline virtual void MoveBy(const Vec2i offset) {
        pos += offset;
        Entity::Move();
    }
    inline virtual void MoveTo(const Vec2f new_pos) {
        pos = new_pos;
        Entity::Move();
    }
    inline virtual void MoveTo(const Vec2i new_pos) {
        pos = new_pos;
        Entity::Move();
    }

    [[nodiscard]] inline Vec2f GetPos() const { return pos; }
    [[nodiscard]] inline Rect GetBBox() const { return bbox; }

protected:
    Vec2f pos;
    Rect bbox;

    static Core* core;
    static Game* game;

    virtual void Move();
};