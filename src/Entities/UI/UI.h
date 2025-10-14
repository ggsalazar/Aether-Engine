#pragma once
#include "../Entity.h" //Collision (Geometry (Vec2 (iostream))), Sprite, Text, Game, Scene
#include "../../Engine/Graphics/Text.h"
#include "../../Game/Menu.h"

class UI : public Entity {
public:
    Menu* menu = nullptr;
    Text label;

    UI(const Sprite::Info& s_i, Menu* m = nullptr, const Widget w = Widget::NONE);

    void GetInput() override;
    void Draw() override;

    bool Selected();

    void SetActive(const bool new_active = true);
    [[nodiscard]] inline bool GetActive() const { return active; }
    [[nodiscard]] inline bool GetActivated() const { return activated; }
    [[nodiscard]] inline Widget GetWidget() const { return widget; }

protected:
    //Variables
    int label_offset = 0;
    Widget widget = Widget::NONE;
    bool active = true, primed = false, activated = false;

    //Functions
    virtual void Pressed();
    virtual void Released() {}
};