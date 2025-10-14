#include "Button.h"

void Button::Update() {
    //When not Selected or primed, this is 0; when Selected but not primed, this is 1; when selected and primed, this is 2
    sprite.SetCurrFrame(Selected() + primed);
}

void Button::Draw() {
    UI::Draw();

    if (sprite.GetSheet() == "UI/Btn_Blank")
        engine->renderer.DrawTxt(label);
}

void Button::Pressed() {
    UI::Pressed();
}

void Button::Released() {
    activated = true;
    switch (widget) {

    case Widget::Apply:
        if (menu->GetName() == MenuName::Options) {
            //Set the game's current resolution to the scale determined by the resolution picker OR set it to fullscreen if that toggle is clicked
            if (menu->GetWidgetStatus(Widget::Fullscreen) == "True" and engine->resolution != engine->window.ScreenSize())
                engine->SetResolution(engine->window.ScreenSize());
            else {
                uint new_scale = stoi(menu->GetWidgetStatus(Widget::Resolution));
                uint old_scale = engine->resolution.x / engine->min_res.x;
                if (new_scale != old_scale)
                    engine->SetResolution(new_scale);
            }
            //Resize all the text
            game->Resize();

            SetActive(false);
        }
        break;

    case Widget::Back:
        menu->Open(false);
        break;

    case Widget::Options:
        menu->Open(false);
        break;

    case Widget::Quit:
        engine->window.open = false;
        break;

    case Widget::Resume:
        menu->Open(false);
        game->paused = false;
        break;

    case Widget::Title:
        break;
    }
}
