#pragma once
#include <vector>
#include "UI.h"

class Picker : public UI {
public:
    template<typename T>
    Picker(const Vec2<T> init_pos, Menu* m, const Widget w);

    void Update() override;
    void Draw() override;

    inline void SetPickingF() { picking.font = label.font; }

    template<typename T>
    inline void MoveBy(Vec2<T> offset) { Entity::MoveBy(offset); Move(); }
    template<typename T>
    inline void MoveTo(Vec2<T> new_pos) { Entity::MoveTo(new_pos); Move(); }

    void SetPicking(const string& new_p);
    inline string GetPicking() const { return picking.GetStr(); }

private:
    //Arrow sprites
    Sprite l_arrow;
    Sprite r_arrow;

    Rect l_bbox;
    bool l_primed = false;
    Rect r_bbox;
    bool r_primed = false;

    Text picking;
    vector<string> options;
    vector<string>::iterator option_picked;

    void Move() override;

    bool LeftSelected();
    inline void LeftPressed() { l_primed = true; }
    void LeftReleased();

    bool RightSelected();
    inline void RightPressed() { r_primed = true; }
    void RightReleased();
};