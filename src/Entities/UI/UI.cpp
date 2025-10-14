#include "UI.h"
#include "../../Engine/Input.h" //Window

UI::UI(const Sprite::Info& s_i, Menu* m, const Widget w)
    : Entity(s_i), menu(m), label(18), widget(w) {

    //Label
    string l_str;
    Sprite::Info info = {}; info.sheet = "UI/Btn_Blank";
    info.pos = pos; info.origin = sprite.GetOrigin();
    info.frame_size = {112, 33}; info.num_frames = 3;
    info.anim_fps = 0; info.dfc = -10;

    switch (widget) {
    case Widget::Apply:
        l_str = "Apply";
        SetActive(false);
        break;

    case Widget::Back:
        l_str = "Back";
        break;


    case Widget::Options:
        l_str = "Options";
        break;

    case Widget::Quit:
        l_str = "Quit";
        break;

    case Widget::Resume:
        l_str = "Resume";
        break;

    case Widget::Title:
        l_str = "Return to Title";
        break;
    }
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
        break;

        case Widget::SFX_V:
            l_str = "SFX Volume";
            info.sheet = "UI/Slider";
        break;

        //Toggles
        case Widget::Fullscreen:
            l_str = "Fullscreen";
            info.sheet = "UI/Toggle";
        break;
    }

    sprite.Init(info);

    //Set bbox size
    Entity::Move();

    label.MoveTo(pos);
    label.SetStr(l_str);
    label.SetOrigin();

    //Sound
    /*
    if (!sb.loadFromFile("assets/SFX/ButtonClick.mp3")) {
        cerr << "Failed to load sound for UI widgetent!" << endl;
        return;
    }
    sound.setBuffer(sb);
    sound.setVolume(game->GetSFXVolume());
    */

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
