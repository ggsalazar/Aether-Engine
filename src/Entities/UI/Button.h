#pragma once
#include "UI.h"

class Button : public UI {
public:
    Button(const Vec2f init_pos, Menu* m, const Widget w);

    void Update() override;
    void Draw() override;
    inline void MoveBy(Vec2f offset) override { Entity::MoveBy(offset); Move(); }
    inline void MoveTo(Vec2f new_pos) override { Entity::MoveTo(new_pos); Move(); }

private:
    void Move() override { label.MoveTo({ pos.x, pos.y - label_offset }); }
    void Pressed() override;
    void Released() override;
};