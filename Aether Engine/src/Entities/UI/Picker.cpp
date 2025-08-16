#include "Picker.h"

Picker::Picker(const Sprite::Info& s_i, Menu* m, const UIElem e,
    const uchar init_ui_layer)
    : UI(s_i, m, e, init_ui_layer), picking(label.font) {

    label_offset = 6;
    label.MoveTo({ pos.x, pos.y - label_offset });

    //Set up bbox
    l_bbox.x = bbox.x + bbox.w * .05;
    l_bbox.y = bbox.y + bbox.h * .1;
    l_bbox.w = bbox.w * .25;
    l_bbox.h = bbox.h * .75;

    //Right bbox
    r_bbox.x = bbox.x + bbox.w * .7;
    r_bbox.y = bbox.y + bbox.h * .1;
    r_bbox.w = bbox.w * .25;
    r_bbox.h = bbox.h * .75;

    //What exactly ARE we picking?
    string picking_str = "";
    switch (elem) {
        case UIElem::Resolution:
            picking_str = to_string(game->resolution.x / game->min_res.x);
        break;
    }
    picking.SetStr(picking_str);
    picking.SetOrigin();
    picking.MoveTo({ pos.x, pos.y + 2 });
}

void Picker::GetInput() {
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
}

void Picker::Draw() {
    if (Selected())
        game->renderer.DrawRect(bbox, Color(0, 1, 0));

    UI::Draw();

    game->renderer.DrawTxt(picking);

    if (LeftSelected())
        game->renderer.DrawRect(l_bbox, Color(1, 0, 0, .5)); //Red, 50% opacity
    else if (RightSelected())
        game->renderer.DrawRect(r_bbox, Color(0, 0, 1, .5)); //Blue, 50% opacity
}

void Picker::Move() {
    //Entity::Move() takes care of sprite & bbox

    //Move everything else

    //l/r bboxes
    l_bbox.x = bbox.x + bbox.w * .05;
    l_bbox.y = bbox.y + bbox.h * .1;

    r_bbox.x = bbox.x + bbox.w * .7;
    r_bbox.y = l_bbox.y;

    label.MoveTo({ pos.x, pos.y - label_offset });

    picking.MoveTo(pos);
}

void Picker::SetPicking(const string new_p) {
    picking.SetStr(new_p);
}

void Picker::LeftReleased() {
    string p = picking.GetStr();
    switch (elem) {
        case UIElem::Resolution: {
            uint curr_res = stoi(p);

            if (--curr_res < 1)
                curr_res = floor(game->window.ScreenSize().x / game->min_res.x);

            p = to_string(curr_res);

            //Set the Apply button to active
            menu->SetUIElemActive(UIElem::Apply);
            break;
        }
    }

    picking.SetStr(p);
}

void Picker::RightReleased() {
    string p = picking.GetStr();
    switch (elem) {
        case UIElem::Resolution: {
            uint curr_res = stoi(p);

            if (++curr_res > floor(game->window.ScreenSize().x / game->min_res.x))
                curr_res = 1;

            p = to_string(curr_res);

            //Set the Apply button to active
            menu->SetUIElemActive(UIElem::Apply);
            break;
        }
    }

    picking.SetStr(p);
}
