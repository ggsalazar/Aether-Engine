#include "UI.h"

UI::UI(const Structure& s, Menu& m, const AnimInfo& a_i, const Animation::Transform& t, const UI::Style& style, const int init_dfc) :
	Entity(s, a_i, t, init_dfc), menu(m), elem(style.elem), label(structure.game.default_font) {
    
    //Label
    string l_str = "Default";
    switch (elem) {
        case UIElems::APPLY:
            l_str = "Apply";
            SetActive(false);
        break;

        case UIElems::BACK:
            l_str = "Back";
        break;

        case UIElems::FULLSCREEN:
            l_str = "Fullscreen";
        break;

        case UIElems::LEVSEL:
            l_str = "Level Select";
        break;

        case UIElems::LEV1:
            l_str = "Level 1";
        break;

        case UIElems::LEV2:
            l_str = "Level 2";
        break;

        case UIElems::LEV3:
            l_str = "Level 3";
        break;

        case UIElems::MAINMENU:
            l_str = "Main Menu";
        break;

        case UIElems::MUSICV:
            l_str = "Music Volume";
        break;

        case UIElems::OPTIONS:
            l_str = "Options";
        break;

        case UIElems::QUIT:
            l_str = "Quit";
        break;

        case UIElems::RESOLUTION:
            l_str = "Resolution";
        break;

        case UIElems::RESUME:
            l_str = "Resume";
        break;

        case UIElems::SFXV:
            l_str = "SFX Volume";
        break;
    }
    Util::InitText(label, game.default_font, style.font_size, pos.x, pos.y, l_str);

    //Sound
    {
        if (!sb.loadFromFile("assets/SFX/ButtonClick.mp3")) {
            cerr << "Failed to load sound for UI element!" << endl;
            return;
        }
        sound.setBuffer(sb);
        sound.setVolume(game.GetSFXVolume());
    }
}

void UI::GetInput() {
    if (active and Selected(MOUSEPOS)) {
        if (Input::KeyPressed(LMB))
            Pressed();

        if (Input::KeyReleased(LMB) and primed)
            Released();
    }
    else if (!Selected(MOUSEPOS)) primed = false;
}

void UI::Resize(uint r_s) {
    //Get the old origin
    Vector2f old_origin = anim->GetOrigin();

    //Set sprite w/h
    anim->sprite.setScale({ r_s*1.f, r_s*1.f });
    //Set new sprite origin
    anim->SetOrigin(old_origin.x, old_origin.y);
    //Adjust label size
    label.setCharacterSize(r_s * 16);
    //Reset the origin
    Util::SetTextOrigin(label);
    //Label offset and position are handled in children
}

void UI::SetActive(const bool new_active) {
    active = new_active;
    Color color = anim->sprite.getColor();
    color.a = active ? 255 : 125;
    anim->sprite.setColor(color);
}

void UI::Pressed() {
    primed = true;
    sound.play();
}
