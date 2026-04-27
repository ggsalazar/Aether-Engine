#include "Slider.h"
#include "../../Engine/Math/Math.h"
#include "../../Engine/Input.h" //Window

void Slider::Init(const Vec2f i_pos) {
    Sprite::Info info; info.sheet = "UI/Slider";
    info.pos = i_pos; info.origin = {.5f};
    info.scale.x = 2;
    info.default_layer = sprite.GetDefaultLayer() == LayerName::NONE ? LayerName::UI : sprite.GetDefaultLayer();
    info.layer_order = menu != nullptr ? menu->layer_order+1 : 1;
    label.SetLayerOrder(info.layer_order+1);
    sprite.Init(info);
    MoveTo(sprite.GetPos());

    switch (widget) {
        case Widget::Msc_V:
            label.SetStr("Music Volume");
        break;

        case Widget::SFX_V:
            label.SetStr("SFX Volume");
        break;
    }
    label_offset = 16;
    label.SetOrigin({.5f, .2f});
    label.MoveTo(Vec2{ pos.x, pos.y - label_offset });

    //Set the origin/anchor, scale, and position
    info = {}; info.sheet = "UI/SliderKnob";
    info.origin = {.5f};
    info.default_layer = sprite.GetDefaultLayer();
    info.layer_order = sprite.GetLayerOrder()+1;
    knob_spr.Init(info);
    knob_pos_max = bbox.x + round(bbox.w * .9f);
    knob_pos_min = bbox.x + round(bbox.w * .1f);

    //Setting knob position based on appropriate value
    string rounded_val;
    if (widget == Widget::Msc_V or widget == Widget::SFX_V) {
        float vol = widget == Widget::Msc_V ? game->dj.GetVolume() : core->GetSFXVolume();
        knob_pos = ((knob_pos_max - knob_pos_min) * vol * .005f) + knob_pos_min;

        //Set the value
        rounded_val = to_string((float)(knob_pos - knob_pos_min) / (float)(knob_pos_max - knob_pos_min) * 200);
        rounded_val = rounded_val.substr(0, rounded_val.find('.')) + '%';
    }
    knob_spr.MoveTo(Vec2{ knob_pos, pos.y });
    knob_label.SetOrigin();
    knob_label.SetStr(rounded_val);
    knob_label.SetLayerOrder(knob_spr.GetLayerOrder()+1);
    knob_label.MoveTo(Vec2{ pos.x, pos.y + label_offset });

    //Move stuff last
    MoveTo(sprite.GetPos());
}

void Slider::Update() {
    UI::Update();

    if (Selected() and Input::BtnDown(LMB)) {

        //Adjust knob position
        knob_pos = Input::MousePos().x;
        Math::Clamp(knob_pos, knob_pos_min, knob_pos_max);
        knob_spr.MoveTo(Vec2{ knob_pos, pos.y });

        float new_val = 0;
        if (widget == Widget::Msc_V or widget == Widget::SFX_V) {
            new_val = (knob_pos - knob_pos_min) / (knob_pos_max - knob_pos_min) * 200;

            if (widget == Widget::Msc_V)
                game->dj.SetVolume(new_val);
            else if (widget == Widget::SFX_V)
                core->SetSFXVolume(new_val);

            knob_label.SetStr(Math::DecPlace(new_val) + '%');
        }
    }
}

void Slider::Draw() {
    sprite.Draw();
    label.Draw();
    knob_label.Draw();
    knob_spr.Draw();
}

void Slider::Move() {
    //Entity::Move() takes care of sprite and bbox

    //Move everything else
    label.MoveTo(Vec2{ pos.x, pos.y - label_offset });

    //Resize and move the knob
    knob_pos_max = bbox.x + bbox.w * .9f;
    knob_pos_min = bbox.x + bbox.w * .1f;

    if (widget == Widget::Msc_V)
        knob_pos = knob_pos_min + (game->dj.GetVolume() * .005 * (knob_pos_max - knob_pos_min));
    else if (widget == Widget::SFX_V)
        knob_pos = knob_pos_min + (core->GetSFXVolume() * .005 * (knob_pos_max - knob_pos_min));

    knob_spr.MoveTo({knob_pos, pos.y});
    knob_label.MoveTo(Vec2{ pos.x, pos.y + label_offset });
}