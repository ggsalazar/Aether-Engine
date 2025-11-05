#pragma once
#include "UI.h"

class Toggle : public UI {
public:
    bool on = false;

    Toggle(const Vec2f init_pos, Menu* m, const Widget w);

    void Draw() override;
    inline void MoveBy(Vec2f offset) override { Entity::MoveBy(offset); Move(); }
    inline void MoveTo(Vec2f new_pos) override { Entity::MoveTo(new_pos); Move(); }

private:
    void Move() override;
    //void Pressed() override;
    void Released() override;
};