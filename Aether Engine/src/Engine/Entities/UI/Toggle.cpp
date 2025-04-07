#include "Toggle.h"

void Toggle::Draw(const bool debug) {
	if (active and Selected(MOUSEPOS))
		window.draw(bbox_debug);

	Entity::Draw(debug);
	window.draw(label);

	if (on)
		anim->SetCurrFrame(1);
	else anim->SetCurrFrame(0);
}

void Toggle::Move() {
	Entity::Move();
	label.setCharacterSize(game.GetResScale() * 12);
	label_offset = game.GetResScale() * 5;
	Util::SetTextOrigin(label, 1.f);
	label.setPosition({ pos.x - label_offset, pos.y });
}

void Toggle::Released() {
	on = !on;
	//Set the Apply button to active
	menu.SetUIElemActive(UIElems::APPLY);

	menu.SetUIElemActive(UIElems::RESOLUTION, !on);
}