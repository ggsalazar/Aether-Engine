#include "Picker.h"

Picker::Picker(const Entity::Structure& s, Menu& m, const Animation::AnimInfo& a_i, const Animation::Transform& t, const UI::Style& style, const int init_dfc) :
    UI(s, m, a_i, t, style, init_dfc), picking(game.default_font) {

    label_offset = game.GetResScale()*5;
    label.setPosition({ pos.x, pos.y - label_offset });

    //Set up position.x bbox
    l_bbox.position.x = bbox.position.x + bbox.size.x * .05;
    l_bbox.position.y = bbox.position.y + bbox.size.y * .1;
    l_bbox.size.x = bbox.size.x*.25;
    l_bbox.size.y = bbox.size.y*.75;
    //Set position.x bbox debug
    l_bbox_debug.setPosition(Vector2f(l_bbox.position.x, l_bbox.position.y));
    l_bbox_debug.setSize(Vector2f(l_bbox.size.x, l_bbox.size.y));
    l_bbox_debug.setFillColor(Color(255, 0, 0, 127)); //Red, 50% opacity

    //Right bbox
    r_bbox.position.x = bbox.position.x + bbox.size.x*.7;
    r_bbox.position.y = bbox.position.y + bbox.size.y * .1;
    r_bbox.size.x = bbox.size.x * .25;
    r_bbox.size.y = bbox.size.y * .75;
    //Right bbox debug
    r_bbox_debug.setPosition(Vector2f(r_bbox.position.x, r_bbox.position.y));
    r_bbox_debug.setSize(Vector2f(r_bbox.size.x, r_bbox.size.y));
    r_bbox_debug.setFillColor(Color(0, 0, 255, 127)); //Blue, 50% opacity

    //What exactly ARE we picking?
    picking = label;
    if (elem == UIElems::RESOLUTION) {
        picking.setString(to_string((int)game.GetResScale()));
        Util::SetTextOrigin(picking);
        picking.setPosition(pos);
    }
}

void Picker::GetInput() {
    if (active) {
        if (LeftSelected(MOUSEPOS)) {
            if (Input::KeyPressed(LMB))
                LeftPressed();
            if (Input::KeyReleased(LMB) and l_primed)
                LeftReleased();
        }
        else if (!LeftSelected(MOUSEPOS)) l_primed = false;
        
        if (RightSelected(MOUSEPOS)) {
            if (Input::KeyPressed(LMB))
                RightPressed();
            if (Input::KeyReleased(LMB) and r_primed)
                RightReleased();
        }
        else if (!RightSelected(MOUSEPOS)) r_primed = false;
    }
}

void Picker::Draw(const bool debug) {
    if (active and Selected(MOUSEPOS))
        window.draw(bbox_debug);

    Entity::Draw(debug);
    window.draw(label);

    window.draw(picking);

    if (active) {
        if (LeftSelected(MOUSEPOS))
            window.draw(l_bbox_debug);
        else if (RightSelected(MOUSEPOS))
            window.draw(r_bbox_debug);
    }
}

void Picker::Move() {
    Entity::Move();
    label_offset = game.GetResScale() * 5;
    label.setPosition({ pos.x, pos.y - label_offset });

    //Move position.x bbox+debug
    l_bbox.position.x = bbox.position.x + bbox.size.x * .05;
    l_bbox.position.y = bbox.position.y + bbox.size.y * .1;
    l_bbox.size.x = bbox.size.x * .25;
    l_bbox.size.y = bbox.size.y * .75;

    l_bbox_debug.setPosition(Vector2f(l_bbox.position.x, l_bbox.position.y));
    l_bbox_debug.setSize(Vector2f(l_bbox.size.x, l_bbox.size.y));
    l_bbox_debug.setFillColor(Color(255, 0, 0, 127)); //Red, 50% opacity

    //Right bbox+debug
    r_bbox.position.x = bbox.position.x + bbox.size.x * .7;
    r_bbox.position.y = bbox.position.y + bbox.size.y * .1;
    r_bbox.size.x = bbox.size.x * .25;
    r_bbox.size.y = bbox.size.y * .75;

    r_bbox_debug.setPosition(Vector2f(r_bbox.position.x, r_bbox.position.y));
    r_bbox_debug.setSize(Vector2f(r_bbox.size.x, r_bbox.size.y));
    r_bbox_debug.setFillColor(Color(0, 0, 255, 127)); //Blue, 50% opacity

    //What exactly ARE we picking?
    picking = label;
    if (elem == UIElems::RESOLUTION) {
        picking.setString(to_string((int)game.GetResScale()));
        Util::SetTextOrigin(picking);
        picking.setPosition(pos);
    }
}

int Picker::GetPicking() {
    string pckng = picking.getString();
    int p = stoi(pckng);
    return p;
}

void Picker::LeftReleased() {
    if (elem == UIElems::RESOLUTION) {
        string c_r = picking.getString();
        uint curr_res = stoi(c_r);
        
        if (--curr_res < 1)
            curr_res = floor(SCREENW() / MINRESW);

        picking.setString(to_string(curr_res));

        //Set the Apply button to active
        menu.SetUIElemActive(UIElems::APPLY);
    }
}

void Picker::RightReleased() {
    if (elem == UIElems::RESOLUTION) {
        string c_r = picking.getString();
        uint curr_res = stoi(c_r);

        if (++curr_res > floor(SCREENW() / MINRESW))
            curr_res = 1;

        picking.setString(to_string(curr_res));

        //Set the Apply button to active
        menu.SetUIElemActive(UIElems::APPLY);
    }
}
