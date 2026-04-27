#pragma once
#include "UI.h"

class Button : public UI {
public:
    template<typename T=float>
    Button(const Vec2<T> init_pos, Menu* m, const Widget w) : UI(m, w) { Init(init_pos)}
    void Init(const Vec2f i_pos);

    void Update() override;
    void Draw() override;
    inline void MoveBy(const Vec2f offset) override { Entity::MoveBy(offset); Move(); }
    inline void MoveBy(const Vec2i offset) override { Entity::MoveBy(offset); Move(); }
    inline void MoveTo(const Vec2f new_pos) override { Entity::MoveTo(new_pos); Move(); }
    inline void MoveTo(const Vec2i new_pos) override { Entity::MoveTo(new_pos); Move(); }

private:
    void Move() override { label.MoveTo(Vec2{ pos.x, pos.y - label_offset }); }
    void Pressed() override;
    void Released() override;
};