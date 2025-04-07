#pragma once
#include "UI.h"

class Button : public UI {
public:
    Button(const Entity::Structure& s, Menu& m, const Animation::AnimInfo& a_i, const Animation::Transform& t = {}, const UI::Style& style = {}, const int init_dfc=0) :
        UI(s, m, a_i, t, style, init_dfc) {

        label_offset = game.GetResScale();
        label.setPosition({ pos.x, pos.y - label_offset });
    }

    void Draw(const bool debug = false) override;
    void Move() override;

private:
    //Functions
    void Pressed() override;
    void Released() override;
};