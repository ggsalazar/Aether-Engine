#include "Toggle.h"

Toggle::Toggle(const Sprite::Info& s_i, Menu* m, const UIElem e,
	const uint init_ui_layer)
	: UI(s_i, m, e, init_ui_layer) {

	label_offset = 12;
	label.SetOrigin({ 1.f, .5 });
	label.MoveTo({ pos.x - label_offset, pos.y });

	switch (elem) {
	case UIElem::Fullscreen:
		on = game->resolution.x == game->window.ScreenSize().x;
		SetActive(!on);
		break;
	}
}

void Toggle::Draw() {
	if (Selected())
		game->renderer.DrawRect(bbox, Color(1, 0, 0));

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