#pragma once
#include "UI.h"

class Toggle : public UI {
public:
    Toggle(const Entity::Structure& s, Menu& m, const Animation::AnimInfo& a_i, const Animation::Transform& t = {}, const UI::Style& style = {}, const int init_dfc = 0) :
        UI(s, m, a_i, t, style, init_dfc) {

        label_offset = game.GetResScale()*5;
        Util::SetTextOrigin(label, 1.f);
        label.setPosition({ pos.x - label_offset, pos.y });
    }

    void Draw(const bool debug = false) override;
    void Move() override;

    bool GetOn() const { return on; }

private:
    bool on = false;

    //Functions
    //void Pressed() override;
    void Released() override;
};