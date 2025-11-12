#pragma once
#include "../Entity.h" //Collision (Geometry (Vec2 (iostream))), Sprite, Text, Game, Scene
#include "../../Engine/Graphics/Text.h"
#include "../../Game/Menu.h"

class UI : public Entity {
public:
    Menu* menu = nullptr;
    Text label;

    UI(Menu* m, const Widget w) : menu(m), label(18), widget(w) {}

    virtual void Update() override;
    inline void Draw() override { Entity::Draw(); }

    bool Selected();

    void SetActive(const bool new_active = true);
    [[nodiscard]] inline bool GetActive() const { return active; }
    [[nodiscard]] inline Widget GetWidget() const { return widget; }

protected:
    int label_offset = 0;
    Widget widget = Widget::NONE;
    bool active = true, primed = false;

    virtual void Pressed();
    virtual void Released() {}
};