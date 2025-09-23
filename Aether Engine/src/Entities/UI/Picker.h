#pragma once
#include <vector>
#include "UI.h"

class Picker : public UI {
public:
    Picker(const Sprite::Info& s_i, Menu* m, const UIElem e);

    void GetInput() override;
    void Draw() override;

    inline void SetPickingF() { picking.font = label.font; }

    inline void MoveBy(Vec2f offset) override { Entity::MoveBy(offset); Move(); }
    inline void MoveTo(Vec2f new_pos) override { Entity::MoveTo(new_pos); Move(); }

    void SetPicking(const string new_p);
    inline string GetPicking() const { return picking.GetStr(); }

private:
    //Variables
    Rect l_bbox;
    bool l_primed = false;
    Rect r_bbox;
    bool r_primed = false;

    Text picking;
    vector<string> options;
    vector<string>::iterator option_picked;

    //Functions
    void Move() override;

    inline bool LeftSelected() { return active and Collision::RectPoint(l_bbox, Input::MousePos()); }
    inline void LeftPressed() { l_primed = true; }
    void LeftReleased();

    inline bool RightSelected() { return active and Collision::RectPoint(r_bbox, Input::MousePos()); }
    inline void RightPressed() { r_primed = true; }
    void RightReleased();
};