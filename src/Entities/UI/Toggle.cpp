#include "Toggle.h"

Toggle::Toggle(const Vec2f init_pos, Menu* m, const Widget w)
	: UI(m, w) {

	Sprite::Info info; info.sheet = "UI/Toggle";
	info.pos = Round(init_pos); info.origin = {.5f};
	info.num_frames = 2; info.frame_size = {24};
	sprite.Init(info);


	label_offset = 12;
	label.SetOrigin({ 1.f, .5 });
	label.MoveTo({ (float)pos.x - label_offset, (float)pos.y });

	switch (widget) {
		case Widget::Fullscreen:
			label.SetStr("Fullscreen");
			on = engine->resolution.x == engine->window.ScreenSize().x;
			SetActive(!on);
		break;
	}

	//Move stuff last
	Toggle::MoveTo(sprite.GetPos());
}

void Toggle::Draw() {
	if (Selected())
		engine->renderer.DrawRect(bbox, Color(1, 0, 0));

	UI::Draw();
	engine->renderer.DrawTxt(label);

	sprite.SetCurrFrame(on);
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