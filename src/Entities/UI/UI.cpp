#include "UI.h"
#include "../../Engine/Collision.h"
#include "../../Engine/Input.h" //Window

UI::UI(Menu *m, const Widget w)
    : menu(m), label(FontName::Normal, 10), widget(w) {
    //UI defaults to the UI layer
    if (sprite.GetDefaultLayer() == LayerName::Objects)
        sprite.SetDefaultLayer(LayerName::UI);
    int lo = menu != nullptr ? menu->layer_order+1 : 1;
    sprite.SetLayerOrder(lo);
    label.SetLayerOrder(lo);
}

void UI::Update() {
    if (Selected()) { //Selected() accounts for current state of active
        if (Input::BtnPressed(LMB))
            Pressed();

        if (Input::BtnReleased(LMB) and primed)
            Released();
    }
    else if (!Selected()) primed = false;
}

void UI::Draw() {
    label.Draw();
}

bool UI::Selected() {
    return active and Collision::RectPoint(bbox, Input::MousePos()) and menu->has_focus;
}

void UI::SetActive(const bool new_active) {
    active = new_active;
    Color new_color = sprite.GetColor();
    new_color.r = active ? 1.f : .5f;
    new_color.g = active ? 1.f : .5f;
    new_color.b = active ? 1.f : .5f;
    sprite.SetColor(new_color);
    label.SetColor(new_color);

    if (!active)
        primed = false;
}

void UI::Move() {
    label.MoveTo(pos);
    bbox.x = label.GetPos().x - label.GetStrSize().x * label.GetOrigin().x;
    bbox.y = label.GetPos().y - label.GetStrSize().y * label.GetOrigin().y;
    bbox.w = label.GetStrSize().x;
    bbox.h = label.GetStrSize().y;
}

void UI::Pressed() {
    primed = true;
    //Play a sound - TO-DO
}