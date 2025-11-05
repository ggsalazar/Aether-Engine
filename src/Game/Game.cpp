#include "Game.h"
#include "Menu.h"
#include "../Engine/Engine.h"
#include "../Engine/Input.h"
#include "../Entities/Entity.h"

Game::~Game() {
	for (auto& e : entities) delete e;
	entities.clear();

	for (auto& m : menus) delete m;
	menus.clear();
}

void Game::Init(Engine* e) {
	//Initialize all the things
	engine = e;
	Menu::SetEngine(engine);
	Entity::SetEngine(engine, this);
	tilemap.SetSDLRenderer(engine->renderer.GetRender());

	//Initialize the cursor sprite
	Sprite::Info spr_info = {};
	spr_info.sheet = "UI/Cursor"; spr_info.frame_size = {10, 14};
	cursor.Init(spr_info);
}

void Game::GetInput() {
	//Update cursor position and frame
	cursor.MoveTo(Input::MousePos());
	cursor.SetCurrFrame(Input::BtnDown(LMB));

	for (auto& e : entities) e->GetInput();

	for (uchar i = 0; i < menus.size(); ++i) menus[i]->GetInput();
}

void Game::Update() {
	//Update cursor position and frame
	cursor.MoveTo(Input::MousePos());
	cursor.SetCurrFrame(Input::BtnDown(LMB));

	//Update everything
	for (auto& e : entities) e->Update();
	//Range-based loop isn't working here
	for (uchar i=0; i<menus.size(); ++i) menus[i]->Update();


	//Sort the entities vector by dfc value every 6th of a second (every 10 game frames) so that entities of a lower dfc value are drawn
	// last (closest to the camera)
	if (engine->GetGameFrames() % 10 == 0) {
		sort(entities.begin(), entities.end(),
			[](const Entity* a, const Entity* b) { return a->sprite.GetDFC() > b->sprite.GetDFC(); });
	}

	//Remove dead entities walking
	for (auto it = entities.begin(); it != entities.end();) {
		Entity* e = *it;
		if (e->expired) it = entities.erase(it);
		else ++it;
	}
}

void Game::Draw() {
	//Draw the map
	if (tilemap.Loaded()) engine->renderer.DrawTilemap(tilemap);

	//Entities
	for (auto& e : entities) e->Draw();
}

void Game::DrawGUI() {
	for (auto& e : entities) e->DrawGUI();

	//Menus are drawn last since they will always be closest to the camera
	for (const auto& m : menus)
		m->Draw();

	//JK lol the cursor is drawn last
	engine->renderer.DrawSprite(cursor);
}

void Game::Resize() {
	for (const auto& m : menus) m->Resize();
}

void Game::ChangeScene(Scene new_scn) {
	curr_scn = new_scn;

	//Wipe the slate clean
	for (auto& m : menus)
		m->Open(false);
	menus.clear();
	entities.clear();
	tilemap.Unload();

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

void Game::OpenMenu(const MenuName menu, const bool o) {
	if (Menu* m = FindMenu(menu))
		m->Open(o);
	else
		cout << "Game::OpenMenu(): Cannot open non-existent menu.\n";

}

Menu* Game::FindMenu(MenuName menu) {
	for (const auto& m : menus)
		if (m->GetName() == menu) return m;

	return nullptr;
}
