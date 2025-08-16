#include "UI.h"

UI::UI(const Sprite::Info& s_i, Menu* m, const UIElem e, const uchar init_ui_layer)
    : Entity(s_i), menu(m), elem(e), ui_layer(init_ui_layer), label(&game->default_fonts[18 * Text::res_scale]) {

    //Label
    string l_str = "";

    //Buttons
    switch (elem) {
    case UIElem::Apply:
        l_str = "Apply";
        SetActive(false);
        break;

    case UIElem::Back:
        l_str = "Back";
        break;

    case UIElem::No:
        l_str = "No";
        break;

    case UIElem::Options:
        l_str = "Options";
        break;

    case UIElem::Quit:
        l_str = "Quit";
        break;

    case UIElem::Resume:
        l_str = "Resume";
        break;

    case UIElem::Title:
        l_str = "Return to Title";
        break;

    case UIElem::Yes:
        l_str = "Yes";
        break;
    }
    //Pickers, sliders, and toggles
    switch (elem) {
        //Pickers

    case UIElem::Resolution:
        l_str = "Resolution";
        break;

        //Sliders
    case UIElem::Music_V:
        l_str = "Music Volume";
        break;

    case UIElem::SFX_V:
        l_str = "SFX Volume";
        break;

        //Toggles
    case UIElem::Fullscreen:
        l_str = "Fullscreen";
        break;
    }

    label.MoveTo(pos);
    label.SetStr(l_str);
    label.SetOrigin();

    sprite.SetDFC(-10);

    //Sound
    /*
    if (!sb.loadFromFile("assets/SFX/ButtonClick.mp3")) {
        cerr << "Failed to load sound for UI element!" << endl;
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
    game->renderer.DrawTxt(label);
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
