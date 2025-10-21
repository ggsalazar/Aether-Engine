#include "Button.h"

Button::Button(const Vec2i init_pos, Menu *m, const Widget w) : UI(m, w) {
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

    sprite.Init(info);

    label_offset = 2;
    label.SetStr(l_str);
    label.SetOrigin();

    Button::Move();
}

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
        game->OpenMenu(MenuName::Main);
        break;

    case Widget::Options:
        menu->Open(false);
        game->OpenMenu(MenuName::Options);
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
