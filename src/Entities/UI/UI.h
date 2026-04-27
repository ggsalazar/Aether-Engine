#pragma once
#include "../Entity.h"
#include "../../Engine/Core.h"
#include "../../Game/Menu.h"

class UI : public Entity {
public:
    Menu* menu = nullptr;
    Text label;

    UI(Menu* m, const Widget w);

    virtual void Update() override;
    inline void Draw() override;

    inline virtual void MoveBy(const Vec2f offset) override { MoveBy(Round(offset)); }
    inline virtual void MoveBy(const Vec2i offset) override {
        pos += offset;
        UI::Move();
    }
    inline virtual void MoveTo(const Vec2f new_pos) override { MoveTo(Round(new_pos)); }
    inline virtual void MoveTo(const Vec2i new_pos) override {
        pos = new_pos;
        UI::Move();
    }

    virtual bool Selected();

    virtual void SetActive(const bool new_active = true);
    [[nodiscard]] inline bool GetActive() const { return active; }
    [[nodiscard]] inline Widget GetWidget() const { return widget; }

protected:
    float label_offset = 0;
    Widget widget = Widget::NONE;
    bool active = true, primed = false;

    void Move();

    virtual void Pressed();
    virtual void Released() {}
};