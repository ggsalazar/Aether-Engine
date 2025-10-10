#include "Toggle.h"

Toggle::Toggle(const Sprite::Info& s_i, Menu* m, const UIElem e)
	: UI(s_i, m, e) {

	label_offset = 12;
	label.SetOrigin({ 1.f, .5 });
	label.MoveTo({ pos.x - label_offset, pos.y });

	switch (elem) {
	case UIElem::Fullscreen:
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

	switch (elem) {
	case UIElem::Fullscreen:
		//Set the Apply button to active
		menu->SetUIElemActive(UIElem::Apply);

		menu->SetUIElemActive(UIElem::Resolution, !on);
		break;

	}
}