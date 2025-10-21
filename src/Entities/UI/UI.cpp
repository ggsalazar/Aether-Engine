#include "UI.h"
#include "../../Engine/Input.h" //Window

UI::UI(Menu* m, const Widget w)
    : menu(m), label(18), widget(w) {


    //Pickers, sliders, and toggles
    switch (widget) {
        //Pickers
        case Widget::Resolution:
            l_str = "Resolution";
        break;

        //Sliders
        case Widget::Music_V:
            l_str = "Music Volume";
            info.sheet = "UI/Slider";
            info.frame_size = {64, 9};
            info.num_frames = 1;
        break;

        case Widget::SFX_V:
            l_str = "SFX Volume";
            info.sheet = "UI/Slider";
            info.frame_size = {64, 9};
            info.num_frames = 1;
        break;

        //Toggles
        case Widget::Fullscreen:
            l_str = "Fullscreen";
            info.sheet = "UI/Toggle";
            info.frame_size = {24};
            info.num_frames = 2;
        break;
    }



    label.MoveTo(pos);
}

void UI::GetInput() {
    if (Selected()) { //Selected() accounts for current state of active
        if (Input::BtnPressed(LMB))
            Pressed();

        if (Input::BtnReleased(LMB) and primed)
            Released();
    }
    else if (!Selected()) primed = false;
}

void UI::Draw() {
    Entity::Draw();
}

bool UI::Selected() {
    return active and Collision::RectPoint(bbox, Input::MousePos()) and menu->has_focus;
}

void UI::SetActive(const bool new_active) {
    active = new_active;
    Color new_color = sprite.GetColor();
    new_color.a = active ? 1.f : .5f;
    sprite.SetColor(new_color);
    label.SetColor(new_color);

    if (!active)
        primed = false;
}

void UI::Pressed() {
    primed = true;
    //sound.play();
}
