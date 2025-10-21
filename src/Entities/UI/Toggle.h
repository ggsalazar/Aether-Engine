#pragma once
#include "UI.h"

class Toggle : public UI {
public:
    bool on = false;

    Toggle(const Vec2i init_pos, Menu* m, const Widget w);

    void Draw() override;
    inline void MoveBy(Vec2i offset) override { Entity::MoveBy(offset); Move(); }
    inline void MoveTo(Vec2i new_pos) override { Entity::MoveTo(new_pos); Move(); }

private:
    //Functions
    void Move() override;
    //void Pressed() override;
    void Released() override;
};