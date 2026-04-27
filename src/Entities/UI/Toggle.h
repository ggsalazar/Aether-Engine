#pragma once
#include "UI.h"

class Toggle : public UI {
public:
    bool on = false;

    template<typename T=float>
    Toggle(const Vec2<T> init_pos, Menu* m, const Widget w) : UI(m, w) { Init(Round(init_pos)); }
    void Init(const Vec2i i_pos);

    void Update() override;
    void Draw() override;
    inline void MoveBy(const Vec2f offset) override { Entity::MoveBy(offset); Move(); }
    inline void MoveBy(const Vec2i offset) override { Entity::MoveBy(offset); Move(); }
    inline void MoveTo(const Vec2f new_pos) override { Entity::MoveTo(new_pos); Move(); }
    inline void MoveTo(const Vec2i new_pos) override { Entity::MoveTo(new_pos); Move(); }

private:
    void Move() override;
    void Released() override;
};