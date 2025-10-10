#pragma once
#include "UI.h"

class Toggle : public UI {
public:
    bool on = false;

    Toggle(const Sprite::Info& s_i, Menu* m, const UIElem e);

    void Draw() override;
    inline void MoveBy(Vec2f offset) override { Entity::MoveBy(offset); Move(); }
    inline void MoveTo(Vec2f new_pos) override { Entity::MoveTo(new_pos); Move(); }

private:
    //Functions
    void Move() override;
    //void Pressed() override;
    void Released() override;
};