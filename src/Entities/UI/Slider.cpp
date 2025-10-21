#include "Slider.h"
#include "../../Engine/Math/Math.h"
#include "../../Engine/Input.h" //Window

Slider::Slider(const Vec2i init_pos, Menu* m, const Widget w)
    : UI(m, w), knob_label(label.GetFontSize()) {

    Sprite::Info info; info.sheet = "UI/Slider";
    info.pos = init_pos; info.origin = {.5f};
    info.scale = 2;
    sprite.Init(info);
    Slider::MoveTo(sprite.GetPos());

    switch (widget) {
        case Widget::Msc_V:
            label.SetStr("Music Volume");
        break;

        case Widget::SFX_V:
            label.SetStr("SFX Volume");
        break;
    }
    label_offset = 6;
    label.SetOrigin();
    label.MoveTo({ pos.x, pos.y - label_offset });

    //Set the origin/anchor, scale, and position
    Sprite::Info knob_info; knob_info.sheet = "UI/SliderKnob";
    knob_info.scale = sprite.GetScale(); knob_info.origin = {.5f};
    knob_spr.Init(knob_info);
    knob_pos_max = bbox.x + bbox.w * .9f;
    knob_pos_min = bbox.x + bbox.w * .1f;

    //Setting knob position based on appropriate value
    string rounded_val;
    if (widget == Widget::Msc_V or widget == Widget::SFX_V) {
        float vol = widget == Widget::Msc_V ? engine->dj.GetVolume() : engine->GetSFXVolume();
        knob_pos = ((knob_pos_max - knob_pos_min) * vol * .005) + knob_pos_min;

        //Set the value
        rounded_val = to_string((knob_pos - knob_pos_min) / (knob_pos_max - knob_pos_min) * 200);
        rounded_val = rounded_val.substr(0, rounded_val.find('.') + 3);
    }
    knob_spr.MoveTo({ (int)knob_pos, pos.y });
    knob_label.SetOrigin();
    knob_label.SetStr(rounded_val + '%');
    knob_label.MoveTo({ pos.x, pos.y + label_offset });

    //Move stuff last
    Slider::MoveTo(sprite.GetPos());
}

void Slider::GetInput() {
    UI::GetInput();

    if (Selected() and Input::BtnDown(LMB)) {

        //Adjust knob position
        knob_pos = Input::MousePos().x;
        Math::Clamp(knob_pos, knob_pos_min, knob_pos_max);
        knob_spr.MoveTo({ (int)knob_pos, pos.y });

        float new_val = 0;
        uchar dec_place = 0;
        if (widget == Widget::Music_V or widget == Widget::SFX_V) {
            new_val = (knob_pos - knob_pos_min) / (knob_pos_max - knob_pos_min) * 200;

            if (widget == Widget::Music_V)
                engine->dj.SetVolume(new_val);
            else if (widget == Widget::SFX_V)
                engine->SetSFXVolume(new_val);

            dec_place = 3;
        }

        //Set the string
        string r_val = to_string(new_val);
        knob_label.SetStr(r_val.substr(0, r_val.find('.') + dec_place) + '%');
    }
}

void Slider::Draw() {
    UI::Draw();

    engine->renderer.DrawTxt(label);
    engine->renderer.DrawTxt(knob_label);
    engine->renderer.DrawSprite(knob_spr);
}

void Slider::Move() {
    //Entity::Move() takes care of sprite and bbox

    //Move everything else
    label.MoveTo({ pos.x, pos.y - label_offset });

    //Resize and move the knob
    knob_pos_max = bbox.x + bbox.w * .9f;
    knob_pos_min = bbox.x + bbox.w * .1f;

    if (widget == Widget::Music_V)
        knob_pos = knob_pos_min + (engine->dj.GetVolume() * .005 * (knob_pos_max - knob_pos_min));
    else if (widget == Widget::SFX_V)
        knob_pos = knob_pos_min + (engine->GetSFXVolume() * .005 * (knob_pos_max - knob_pos_min));

    knob_spr.MoveTo(Round(knob_pos, pos.y));
    knob_label.MoveTo({ pos.x, pos.y + label_offset });
}
