#include "Toggle.h"

Toggle::Toggle(const Sprite::Info& s_i, Menu* m, const Widget w)
	: UI(s_i, m, w) {

	label_offset = 12;
	label.SetOrigin({ 1.f, .5 });
	label.MoveTo({ pos.x - label_offset, pos.y });

	switch (widget) {
	case Widget::Fullscreen:
		on = engine->resolution.x == engine->window.ScreenSize().x;
		SetActive(!on);
		break;
	}
}

void Toggle::Draw() {
	if (Selected())
		engine->renderer.DrawRect(bbox, Color(1, 0, 0));

	UI::Draw();

	if (on)
		sprite.SetCurrFrame(1);
	else sprite.SetCurrFrame(0);
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