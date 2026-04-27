#include "Toggle.h"


void Toggle::Init(const Vec2f i_pos) {

	Sprite::Info info; info.sheet = "UI/Toggle";
	info.pos = i_pos; info.origin = {.5f};
	info.default_layer = sprite.GetDefaultLayer() == LayerName::NONE ? LayerName::UI : sprite.GetDefaultLayer();
	info.layer_order = menu != nullptr ? menu->layer_order+1 : 1;
	sprite.Init(info);

	label_offset = 12;
	label.SetOrigin({ 1.f, .5 });
	label.SetLayerOrder(info.layer_order+1);
	label.MoveTo(Vec2{ pos.x - label_offset, pos.y });

	switch (widget) {
		case Widget::Fullscreen:
			label.SetStr("Fullscreen");
			on = core->resolution.x == core->window.GetScreenSize().x;
			SetActive(!on);
		break;
	}

	//Move stuff last
	Toggle::MoveTo(sprite.GetPos());
}

void Toggle::Update() {
	UI::Update();

	if (!on)
		sprite.SetFrame(Selected() or primed);
	else sprite.SetFrame(2 + (Selected() or primed));
}

void Toggle::Draw() {
	sprite.Draw();
	label.Draw();
}

void Toggle::Move() {
	label.MoveTo({ pos.x - label_offset, pos.y });
}

void Toggle::Released() {
	on = !on;

	switch (widget) {
		case Widget::Fullscreen:
			//Set the Apply button to active
			menu->SetWidgetActive(Widget::Apply);

			menu->SetWidgetActive(Widget::Resolution, !on);
		break;

	}
}