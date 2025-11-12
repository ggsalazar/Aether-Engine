#include "Picker.h"
#include "../../Engine/Collision.h"
#include "../../Engine/Input.h"

template<typename T>
Picker::Picker(const Vec2<T> init_pos, Menu* m, const Widget w)
    : UI(m, w), picking(label.GetFontSize()) {

    Sprite::Info info; info.sheet = "UI/Btn_Blank";
    info.pos = Round(init_pos); info.origin = {.5f};
    info.num_frames = 3; info.frame_size = {112, 33};
    sprite.Init(info);

    //We want those arrow sprites
    Sprite::Info arrow_info;
    arrow_info.sheet = "UI/Picker_Arrow"; arrow_info.origin = {.5f};
    arrow_info.frame_size = {15, 10}; arrow_info.num_frames = 2;
    l_arrow.Init(arrow_info); r_arrow.Init(arrow_info);

    label_offset = 6;
    label.SetOrigin();
    label.MoveTo(Vec2i{ pos.x, pos.y - label_offset });

    //What exactly ARE we picking?
    switch (widget) {
        case Widget::Resolution:
            label.SetStr("Resolution");
            picking.SetStr(to_string(engine->resolution.x / engine->min_res.x));
        break;
    }
    picking.SetOrigin();

    //Move stuff last
    MoveTo(sprite.GetPos());
}

void Picker::Update() {
    UI:Update();

    if (active) {
        if (LeftSelected()) {
            if (Input::BtnPressed(LMB))
                LeftPressed();
            if (Input::BtnReleased(LMB) and l_primed)
                LeftReleased();
        }
        else if (!LeftSelected()) l_primed = false;

        if (RightSelected()) {
            if (Input::BtnPressed(LMB))
                RightPressed();
            if (Input::BtnReleased(LMB) and r_primed)
                RightReleased();
        }
        else if (!RightSelected()) r_primed = false;
    }

    //Change the current frame of the arrows
    l_arrow.SetCurrFrame(LeftSelected());
    r_arrow.SetCurrFrame(RightSelected());
    //And ourself
    sprite.SetCurrFrame(Selected() + primed);
}

void Picker::Draw() {
    UI::Draw();

    if (sprite.GetSheet() == "UI/Btn_Blank")
        engine->renderer.DrawTxt(label);

    //Draw the arrows
    engine->renderer.DrawSprite(l_arrow);
    engine->renderer.DrawSprite(r_arrow);

    engine->renderer.DrawTxt(picking);

    if (LeftSelected())
        engine->renderer.DrawRect(l_bbox, Color(1, 0, 0, .5)); //Red, 50% opacity
    else if (RightSelected())
        engine->renderer.DrawRect(r_bbox, Color(0, 0, 1, .5)); //Blue, 50% opacity
}

void Picker::Move() {
    //Entity::Move() takes care of sprite & bbox

    //Move everything else

    //l/r bboxes
    //Set up left bbox
    l_bbox.x = bbox.x + round(bbox.w * .025);
    l_bbox.y = bbox.y + round(bbox.h * .15);
    l_bbox.w = round(bbox.w * .2);
    l_bbox.h = round(bbox.h * .75);
    l_arrow.SetScale({-1, 1});
    l_arrow.MoveTo(Vec2f{l_bbox.x + l_bbox.w*.5f, l_bbox.y + l_bbox.h*.5f});

    //Right bbox
    r_bbox.x = bbox.x + round(bbox.w * .775);
    r_bbox.y = l_bbox.y;
    r_bbox.w = l_bbox.w;
    r_bbox.h = l_bbox.h;
    r_arrow.MoveTo(Vec2f{r_bbox.x + r_bbox.w*.5f, r_bbox.y + r_bbox.h*.5f});

    label.MoveTo(Vec2i{ pos.x, pos.y - label_offset });
    picking.MoveTo(Vec2i{pos.x, pos.y + label_offset});
}

void Picker::SetPicking(const string& new_p) {
    picking.SetStr(new_p);
}

bool Picker::LeftSelected() {
    return active and Collision::RectPoint(l_bbox, Input::MousePos());
}

void Picker::LeftReleased() {
    uint curr_picking = stoi(picking.GetStr());

    switch (widget) {
        case Widget::Resolution:
            if (--curr_picking < 1)
                curr_picking = floor(engine->window.ScreenSize().x / engine->min_res.x);

            //Set the Apply button to active
            menu->SetWidgetActive(Widget::Apply);
        break;
    }

    picking.SetStr(to_string(curr_picking));
}

bool Picker::RightSelected() {
    return active and Collision::RectPoint(r_bbox, Input::MousePos());
}

void Picker::RightReleased() {
    uint curr_picking = stoi(picking.GetStr());

    switch (widget) {
        case Widget::Resolution:
            if (++curr_picking > floor(engine->window.ScreenSize().x / engine->min_res.x))
                curr_picking = 1;
            //Set the Apply button to active
            menu->SetWidgetActive(Widget::Apply);
        break;
    }

    picking.SetStr(to_string(curr_picking));
}