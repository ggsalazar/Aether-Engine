#include <algorithm>
#include "Scene.h"
#include "Menu.h"
#include "../Entities/Entity.h"
#include "../Entities/UI/UI.h"

void Scene::GetInput() {
	for (auto& e : entities) {
		//Only get input for UI elements if the corresponding menu is open
		if (auto ui = dynamic_cast<UI*>(e.get())) {
			if (ui->menu.open)
				ui->GetInput();
		}
		else
			e->GetInput();
	}
}

void Scene::Update() {
	for (auto& e : entities)
		e->Update();

	//Remove dead entities
	entities.erase(remove_if(entities.begin(), entities.end(),
					[](const shared_ptr<Entity>& e) { return !e->alive;}),
			entities.end());

	//Sort the entities vector (and possibly Menus map) by dfc value every 6th of a second so that entities of a lower dfc value are drawn
	// last (closest to the camera)
	if (game.GetFramesElapsed() % 10 == 0)
		sort(entities.begin(), entities.end(), [](const shared_ptr<Entity>& a, const shared_ptr<Entity>& b) { return a->dfc > b->dfc; });
}

void Scene::Draw() {

	for (auto& e : entities) {
		//Only draw UI elements if the corresponding menu is open
		if (auto ui = dynamic_cast<UI*>(e.get())) {
			if (ui->menu.open)
				ui->Draw();
		}
		else
			e->Draw();
	}

	//Menus are drawn last since UI will always be closest to the camera
	//To solve dfc problem, may have to just give Menus their own dfc
	for (const auto& m : menus)
		m.second->Draw();
}

//Most *actual* initialization is handled here
void Scene::Open() {
	open = true;
	entities.clear();

	float res_scalar = game.GetResScale();

	//Each scene is comprised of Menus & Entities
	if (label == Scenes::TITLE) {
	}
	else if (label == Scenes::GAME) {
	}
}

void Scene::Close() {
	open = false;
	//This also deletes the buttons that belong to each menu except for the button that actually changed the scene,
	// which now belongs to the new scene and is immediately deleted when that scene is opened
	//Should I make an "entities_to_keep" vector?
	for (const auto& m : menus)
		m.second->open = false;
	menus.clear();
	entities.clear();
}

void Scene::OpenMenu(Menus menu) {
	for (const auto& m : menus) {
		if (m.first == menu) {
			m.second->open = true;
			break;
		}
	}
}

bool Scene::MenuOpen(Menus menu) {
	for (const auto& m : menus) {
		if (m.first == menu) {
			return m.second->open;
			break;
		}
	}
	cout << "That Menu does not exist in this Scene " << endl;
	return false;
}

void Scene::CloseMenu(Menus menu) {
	for (const auto& m : menus) {
		if (m.first == menu) {
			m.second->open = false;
			break;
		}
	}
}

void Scene::ResizeMenus() {
	for (const auto& m : menus)
		m.second->Resize();
}
