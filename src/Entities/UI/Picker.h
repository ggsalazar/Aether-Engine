#pragma once
#include <vector>
#include "UI.h"

class Picker : public UI {
public:
    Picker(const Sprite::Info& s_i, Menu* m = nullptr, const Widget w=Widget::NONE);

    void GetInput() override;
    void Update() override;
    void Draw() override;

    inline void SetPickingF() { picking.font = label.font; }

    inline void MoveBy(Vec2f offset) override { Entity::MoveBy(offset); Move(); }
    inline void MoveTo(Vec2f new_pos) override { Entity::MoveTo(new_pos); Move(); }

    void SetPicking(const string& new_p);
    inline string GetPicking() const { return picking.GetStr(); }

private:
    //Arrow sprites
    Sprite l_arrow;
    Sprite r_arrow;

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

    inline bool LeftSelected();
    inline void LeftPressed() { l_primed = true; }
    void LeftReleased();

    inline bool RightSelected();
    inline void RightPressed() { r_primed = true; }
    void RightReleased();
};