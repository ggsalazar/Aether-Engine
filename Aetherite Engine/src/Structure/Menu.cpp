#include "Menu.h"
#include "Game.h"
#include "../Entities/UI/Button.h"
#include "../Entities/UI/Slider.h"
#include "../Entities/UI/Picker.h"
#include "../Entities/UI/Toggle.h"

Menu::Menu(Game& g, sf::RenderWindow& w, Scene& s, Menus init_label) :
            game(g), window(w), scene(s), label(init_label), menu_text(game.default_font) {
    float res_scalar = game.GetResScale() * .5;

    Util::InitText(menu_text, game.default_font, res_scalar * 10, window.getSize().x * .5, window.getSize().y * .3, "DEFAULT");
    
    //What we do depends on our label
    switch (label) {
        case Menus::GO: {
            break;
        }

        case Menus::LEVSEL: {
            break;
        }

        case Menus::MAIN: {
            break;
        }

        case Menus::OPTIONS: {
            break;
        }

        case Menus::PAUSE: {
            break;
        }
    }
    
    //Add our UI elements to the scene entities vector
    for (const auto& ui : ui_elems)
        scene.AddEntity(ui.second);
}

void Menu::Resize() {

    //Resize all text and UI elements according to the new res scalar and window dimensions
    float res_scalar = game.GetResScale();
    float w_x = window.getSize().x;
    float w_y = window.getSize().y;

    //A more efficient way to do this would be to base the new size on the old size and do some funky math shit with the scalar, but this works for now
    menu_text.setCharacterSize(res_scalar * 10);
    Util::SetTextOrigin(menu_text);
    menu_text.setPosition({ w_x * .5f, w_y * .2f });
    if (label == Menus::MAIN) {
        menu_text.setCharacterSize(res_scalar * 15);
        Util::SetTextOrigin(menu_text);
        menu_text.setPosition({ w_x * .5f, w_y * .1f });
    }

    //UI Elements
    for (const auto& ui : ui_elems) {
        auto& u = ui.second;

        u->Resize(res_scalar);

        //Adjust the positions
        switch (ui.first) {
            case UIElems::APPLY:
                u->pos.x = w_x * .5;
                u->pos.y = w_y * .8;
            break;

            case UIElems::BACK:
                u->pos.x = w_x * .5;
                u->pos.y = w_y * .9;
            break;

            case UIElems::FULLSCREEN:
                u->pos.x = w_x * (.5 + res_scalar * .02);
                u->pos.y = w_y * .675;
            break;

            case UIElems::LEVSEL:
                u->pos.x = w_x * .5;
                u->pos.y = w_y * .5;
            break;

            case UIElems::LEV1:
                u->pos.x = w_x * .3;
                u->pos.y = w_y * .5;
            break;

            case UIElems::LEV2:
                u->pos.x = w_x * .5;
                u->pos.y = w_y * .5;
            break;

            case UIElems::LEV3:
                u->pos.x = w_x * .7;
                u->pos.y = w_y * .5;
            break;

            case UIElems::MUSICV:
                u->pos.x = w_x * .5;
                u->pos.y = w_y * .3;
            break;

            case UIElems::OPTIONS:
                u->pos.x = w_x * .5;
                u->pos.y = w_y * .6;
            break;

            case UIElems::QUIT:
                u->pos.x = w_x * .5;
                u->pos.y = w_y * .7;
            break;

            case UIElems::RESOLUTION:
                u->pos.x = w_x * .5;
                u->pos.y = w_y * .55;
            break;

            case UIElems::SFXV:
                u->pos.x = w_x * .5;
                u->pos.y = w_y * .425;
            break;
        }
        u->SetBBox();
        u->Move();
    }
}

bool Menu::CheckUIElem(const UIElems ui) {
    if (ui_elems.count(ui) > 0) return true;
    else cout << "That UI Element does not exist!" << endl;
    return false;
}

void Menu::SetUIElemActive(const UIElems ui, const bool a) {
    if (CheckUIElem(ui))
        ui_elems[ui]->SetActive(a);
}

int Menu::GetUIElemStatus(const UIElems ui) {
    if (CheckUIElem(ui)) {
        if (auto picker = dynamic_cast<Picker*>(ui_elems[ui].get()))
            return picker->GetPicking();
        else if (auto toggle = dynamic_cast<Toggle*>(ui_elems[ui].get()))
            return toggle->GetOn();
    }

    return 0;
}

bool Menu::GetUIElemActive(const UIElems ui) {
    if (CheckUIElem(ui))
        return ui_elems[ui]->GetActive();

    return false;
}