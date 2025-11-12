#pragma once
#include "../Engine/Engine.h" //Camera, Renderer, Game (Enums, Sprite)
#include "../Engine/Math/Geometry.h" //Vec2 (iostream)

class Entity {
public:
    Vec2i size;
    inline static uchar SEC;
    bool expired = false;

    explicit Entity(const Sprite::Info& spr_info = {});
    virtual ~Entity() = default;

    static void SetEngine(Engine* e, Game* g);

    inline virtual void Update() { sprite.Update(); sprite.SetDFC(-pos.y); }
    inline virtual void Draw() { sprite.Draw(); }
    inline virtual void DrawGUI() {}

    inline int GetDFC() const { return sprite.GetDFC(); }

    template<typename T>
    void MoveBy(const Vec2<T>& offset);
    template<typename T>
    void MoveTo(const Vec2<T>& new_pos);

    [[nodiscard]] inline Vec2i GetPos() const { return pos; }
    [[nodiscard]] inline Rect GetBBox() const { return bbox; }

protected:
    Vec2i pos;
    Rect bbox;
    Sprite sprite;

    static Engine* engine;
    static Game* game;

    virtual void Move();
};