#pragma once
#include "UI.h"

class Button : public UI {
public:
    template<typename T=float>
    Button(const Vec2<T> init_pos, Menu* m, const Widget w);

    void Update() override;
    void Draw() override;
    template<typename T=float>
    inline void MoveBy(Vec2<T> offset) { Entity::MoveBy(offset); Move(); }
    template<typename T=float>
    inline void MoveTo(Vec2<T> new_pos) { Entity::MoveTo(new_pos); Move(); }

private:
    void Move() override { label.MoveTo(Vec2i{ pos.x, pos.y - label_offset }); }
    void Pressed() override;
    void Released() override;
};