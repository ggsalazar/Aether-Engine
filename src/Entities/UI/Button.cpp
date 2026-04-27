#include "Button.h"
#include "../../Engine/Collision.h"
#include "../../Engine/Input.h"

void Button::Init(const Vec2f i_pos) {
    Sprite::Info info; info.pos = i_pos;
    info.origin = {.5f};
    info.sheet = "UI/BlankBtn";
    info.default_layer = sprite.GetDefaultLayer() == LayerName::NONE ? LayerName::UI : sprite.GetDefaultLayer();
    info.layer_order = menu != nullptr ? menu->layer_order+1 : 1;
    label.SetLayerOrder(info.layer_order+1);

    switch (widget) {
        case Widget::Close:
            info.sheet = "UI/CloseBtn";
        break;

        //All other buttons
        case Widget::Apply:
            label.SetStr("Apply");
            SetActive(false);
            break;

        case Widget::Back:
            label.SetStr("Back");
            break;

        case Widget::Options:
            label.SetStr("Options");
            break;

        case Widget::Quit:
            label.SetStr("Quit");
            break;

        case Widget::Resume:
            label.SetStr("Resume");
            break;

        case Widget::Title:
            label.SetStr("Return to Title");
            break;
    }
    info.tint = sprite.GetColor();
    sprite.Init(info);

    label_offset = 0;
    label.SetOrigin();

    //Move stuff last
    MoveTo(sprite.GetPos());
}

void Button::Update() {
    UI::Update();

    //When not Selected or primed, this is 0; when Selected but not primed, this is 1; when selected and primed, this is 2
    sprite.SetFrame(UI::Selected() + primed);
}

void Button::Draw() {
    sprite.Draw();

    if (sprite.GetSheet() == "UI/Btn_Blank")
        label.Draw();
}

void Button::Pressed() {
    primed = true;
}

void Button::Released() {
    primed = false;
    
    switch (widget) {
        case Widget::Apply:
            if (menu->GetName() == MenuName::Options) {
                //Set the game's current resolution to the scale determined by the resolution picker OR set it to fullscreen if that toggle is clicked
                if (menu->GetWidgetStatus(Widget::Fullscreen) == "True" and core->resolution != core->window.GetScreenSize())
                    core->SetResolution(core->window.GetScreenSize());
                else {
                    uint new_scale = stoi(menu->GetWidgetStatus(Widget::Resolution));
                    uint old_scale = core->resolution.x / core->min_res.x;
                    if (new_scale != old_scale)
                        core->SetResolution(new_scale);
                }
                //Resize all the text
                game->Resize();

                SetActive(false);
            }
        break;

        case Widget::Back:
            menu->to_close = true;
            game->OpenMenu(MenuName::Main);
        break;

        case Widget::Options:
            menu->to_close = true;
            game->OpenMenu(MenuName::Options);
        break;

        case Widget::Quit:
            core->window.open = false;
        break;

        case Widget::Resume:
            menu->to_close = true;
            game->paused = false;
        break;

        case Widget::Title:
        break;
    }
}
