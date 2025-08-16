#pragma once
#include "../Entity.h" //Collision (Geometry (Vec2 (iostream))), Sprite, Text, Game, Scene
#include "../../Core/Input.h" //Window
#include "../../Game/Menu.h"

class UI : public Entity {
public:
    Menu* menu = nullptr;
    uchar ui_layer;
    Text label;

    UI(const Sprite::Info& s_i, Menu* m, const UIElem e,
        const uchar init_ui_layer = 0);

    virtual void GetInput() override;
    virtual void Draw() override;

    inline bool Selected() { return active and Collision::RectPoint(bbox, Input::MousePos()) and game->curr_ui_layer == ui_layer; }

    void SetActive(const bool new_active = true);
    inline bool GetActive() const { return active; }
    inline bool GetActivated() const { return activated; }
    inline UIElem GetElem() const { return elem; }

protected:
    //Variables
    int label_offset = 0;
    UIElem elem = UIElem::NONE;
    bool active = true;
    bool primed = false;
    bool activated = false;

    //Functions
    virtual void Pressed();
    virtual void Released() {}
};