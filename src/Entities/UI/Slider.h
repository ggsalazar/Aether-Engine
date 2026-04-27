#pragma once
#include "UI.h"

class Slider : public UI {
public:
    template<typename T=float>
    Slider(const Vec2<T> init_pos, Menu* m, const Widget w) : UI(m, w), knob_label(FontName::Normal, label.GetFontSize()) {
        Init(Round(init_pos));
    }
    void Init(const Vec2i i_pos);

    void Update() override;
    void Draw() override;

    inline void SetKLF() { knob_label.font = label.font; }

    inline void MoveBy(const Vec2f offset) override { Entity::MoveBy(offset); Move(); }
    inline void MoveBy(const Vec2i offset) override { Entity::MoveBy(offset); Move(); }
    inline void MoveTo(const Vec2f new_pos) override { Entity::MoveTo(new_pos); Move(); }
    inline void MoveTo(const Vec2i new_pos) override { Entity::MoveTo(new_pos); Move(); }

private:
    void Move() override;

    //Knob stuff
    Sprite knob_spr;
    Text knob_label;
    float knob_pos_max = 0, knob_pos_min = 0, knob_pos = 0;
};