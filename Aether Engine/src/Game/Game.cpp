#include "Game.h"
#include "Menu.h"
#include "../Engine/Engine.h"
#include "../Engine/Input.h"
#include "../Engine/Math/Math.h"
#include "../Entities/Entity.h"

void Game::Init(Engine* e) {
	//Initialize all the things
	engine = e;
	Menu::SetEngine(engine);
	Entity::SetEngine(engine, this);
}

void Game::ChangeScene(Scene new_scn) {
	curr_scn = new_scn;

	//Wipe the slate clean
	for (auto& m : menus)
		m->Open(false);
	menus.clear();
	entities.clear();

	switch (curr_scn) {
		case Scene::Title:
			menus.push_back(new Menu(MenuName::Main));
			FindMenu(MenuName::Main)->Open();
			menus.push_back(new Menu(MenuName::Options));
		break;

		case Scene::Game:
		break;
	}
}

void Game::GetInput() {
	for (auto& e : entities) e->GetInput();

	for (uchar i = 0; i < menus.size(); ++i) menus[i]->GetInput();
}

void Game::Update() {
	for (auto& e : entities) e->Update();
	for (auto& m : menus) {
		m->Update();
		if (m->to_close) OpenMenu(m->GetName(), false);
	}

	if (engine->GetGameFrames() % 10 == 0) {
		//Sort the entities vector by dfc value every 6th of a second (every 10 game frames) so that entities of a lower dfc value are drawn
		// last (closest to the camera)
		sort(entities.begin(), entities.end(),
			[](const Entity* a, const Entity* b) { return a->sprite.GetDFC() > b->sprite.GetDFC(); });
	}
}

void Game::Draw() {
	//Entities
	for (const auto& e : entities)
		e->Draw();
}

void Game::DrawGUI() {
	for (const auto& e : entities) e->DrawGUI();

	//Menus are drawn last since they will always be closest to the camera
	for (const auto& m : menus)
		m->Draw();
}

void Game::Resize() {
	for (const auto& m : menus) m->Resize();
}

void Game::OpenMenu(const MenuName menu, const bool o) {
	if (Menu* m = FindMenu(menu))
		m->Open();
	else
		cout << "Game::OpenMenu(): Cannot open non-existent menu.\n";

}

Menu* Game::FindMenu(MenuName menu) {
	for (const auto& m : menus)
		if (m->GetName() == menu) return m;

	return nullptr;
}
