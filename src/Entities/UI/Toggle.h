#pragma once
#include "UI.h"

class Toggle : public UI {
public:
    bool on = false;

    Toggle(const Vec2f init_pos, Menu* m, const Widget w);

    void Draw() override;
    template<typename T>
    inline void MoveBy(Vec2<T>& offset) { Entity::MoveBy(offset); Move(); }
    template<typename T>
    inline void MoveTo(Vec2<T>& new_pos) { Entity::MoveTo(new_pos); Move(); }

private:
    void Move() override;
    void Released() override;
};