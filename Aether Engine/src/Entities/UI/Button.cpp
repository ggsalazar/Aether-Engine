#include <windows.h>
#include <shellapi.h>
#include "Button.h"

void Button::Update() {
}

void Button::Draw() {
    if (Selected())
        engine->renderer.DrawRect(bbox, Color(1, 0, 0)); //THIS IS A TEMPORARY MEASURE

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
            if (menu->GetUIElemStatus(UIElem::Fullscreen) == "True" and engine->resolution != engine->window.ScreenSize())
                engine->SetResolution(engine->window.ScreenSize());
            else {
                uint new_scale = stoi(menu->GetUIElemStatus(UIElem::Resolution));
                uint old_scale = engine->resolution.x / engine->min_res.x;
                if (new_scale != old_scale)
                    engine->SetResolution(new_scale);
            }
            //Resize all the text
            game->Resize();

            SetActive(false);
        }
        break;

    case UIElem::Back:
        menu->Open(false);
        break;

    case UIElem::Options:
        menu->Open(false);
        break;

    case UIElem::Quit:
        engine->window.open = false;
        break;

    case UIElem::Resume:
        menu->Open(false);
        engine->paused = false;
        break;

    case UIElem::Title:
        break;
    }
}
