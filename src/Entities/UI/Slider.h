#pragma once
#include "UI.h"

class Slider : public UI {
public:
    Slider(const Vec2f init_pos, Menu* m, const Widget w);

    void Update() override;
    void Draw() override;

    inline void SetKLF() { knob_label.font = label.font; }

    inline void MoveBy(Vec2f offset) override { Entity::MoveBy(offset); Move(); }
    inline void MoveTo(Vec2f new_pos) override { Entity::MoveTo(new_pos); Move(); }

private:
    void Move() override;

    //Knob stuff
    Sprite knob_spr;
    Text knob_label;
    int knob_pos_max = 0, knob_pos_min = 0, knob_pos = 0;
};