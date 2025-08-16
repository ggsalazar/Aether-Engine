#include <windows.h>
#include <shellapi.h>
#include "Button.h"

void Button::Update() {
}

void Button::Draw() {
    if (Selected())
        game->renderer.DrawRect(bbox, Color(1, 0, 0)); //THIS IS A TEMPORARY MEASURE

    UI::Draw();
}

void Button::Pressed() {
    UI::Pressed();
}

void Button::Released() {
    activated = true;
    switch (elem) {

        case UIElem::Apply:
            if (menu->GetName() == MenuName::Options) {
                //Set the game's current resolution to the scale determined by the resolution picker OR set it to fullscreen if that toggle is clicked
                if (menu->GetUIElemStatus(UIElem::Fullscreen) == "True" and game->resolution != game->window.ScreenSize())
                    game->SetResolution(game->window.ScreenSize());
                else {
                    uint new_scale = stoi(menu->GetUIElemStatus(UIElem::Resolution));
                    uint old_scale = game->resolution.x / game->min_res.x;
                    if (new_scale != old_scale)
                        game->SetResolution(new_scale);
                }
                //Resize all the text
                //gm->Resize();

                SetActive(false);
            }
        break;

        case UIElem::Back:
            //If creating a character, ask to confirm and if yes, wipe party creation - TO-DO
            menu->Open(false);
            //gm->OpenMenu(MenuName::Main);
        break;

        case UIElem::No:
            //Close the confirm menu
            menu->to_close = true;
        break;

    case UIElem::Options:
        menu->Open(false);
        //gm->OpenMenu(MenuName::Options);
        break;

    case UIElem::Quit:
        //gm->CreateMenu(MenuName::Confirm_Quit);
        break;

    case UIElem::Resume:
        menu->Open(false);
        game->paused = false;
        break;

    case UIElem::Title:
        //gm->CreateMenu(MenuName::Confirm_Title);
        break;

    case UIElem::Yes:
        //Confirm Menu closing is handled in GameMaster
        //What exactly did we just confirm?
        switch (menu->GetName()) {
            case MenuName::Confirm_Quit:
                game->window.open = false;
            break;
        }
        break;
    }
}
