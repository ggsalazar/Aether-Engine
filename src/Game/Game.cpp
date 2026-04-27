#include "Game.h"
#include "Menu.h"
#include "../Engine/Core.h"
#include "../Engine/Input.h"
#include "../Entities/Entity.h"

Game::~Game() {
	for (auto& e : entities) delete e;
	entities.clear();

	for (auto& [mn, m] : menus) delete m;
	menus.clear();
}

void Game::Init(Core* c) {
	//Initialize all the things
	core = c;
	camera.viewport.w = core->min_res.x; camera.viewport.h = core->min_res.y;
	Menu::SetCore(core, this);
	Entity::SetCore(core, this);
	tilemap.SetRenderer(&core->renderer);

	//Initialize the cursor sprite
	Sprite::Info spr_info = {};
	spr_info.sheet = "UI/Cursor";
	cursor.Init(spr_info);
}

void Game::Update() {
	cursor.MoveTo(Input::MousePos());

	//Update everything
	for (auto& e : entities) e->Update();
	//Range-based loop not being used because while looping through the menus map, a new menu may be created;
	// this fucks with the iterator and causes a "singular iterator" error. The easiest way I've found to deal with
	// this is to just use a normal for loop
	for (auto& [mn, m] : menus) {
		if (m) m->Update();
	}
	//for (uchar i=0; i<menus.size(); ++i)
	//	menus[i]->Update();

	//Every sixth of a second, kill expired entities and menus
	if (core->GetGameFrames() % (uchar)(Entity::SEC*.16666667f) == 0) {
		for (auto it = entities.begin(); it != entities.end();) {
			Entity* e = *it;
			if (e->expired) it = entities.erase(it);
			else ++it;
		}
		for (auto it = menus.begin(); it != menus.end();) {
			Menu* m = it->second;
			if (m and m->expired) it = menus.erase(it);
			else ++it;
		}
	}
}

void Game::Draw() {
	//Entities
	for (auto& e : entities) e->Draw();
}

void Game::DrawGUI() {
	for (auto& e : entities) e->DrawGUI();

	//Menus are drawn last since they will always be closest to the camera
	for (const auto& [mn, m] : menus)
		m->Draw();

	//JK lol the cursor is drawn last
	cursor.Draw();
}

void Game::ChangeScene(Scene new_scn) {
	curr_scn = new_scn;

	//Wipe the slate clean
	for (auto& [mn, m] : menus)
		m->Open(false);
	menus.clear();
	entities.clear();
	tilemap.Unload();

	switch (curr_scn) {
		case Scene::Title:
			menus.emplace(MenuName::Main, new Menu(MenuName::Main));
			menus.emplace(MenuName::Options, new Menu(MenuName::Options));
			menus[MenuName::Main]->Open();
		break;

		case Scene::Game:
		break;
	}
}