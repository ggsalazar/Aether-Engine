#include "Scene.h"
#include "Menu.h"
#include "Game.h"
#include "../Entities/Entity.h"

void Scene::Init(Game* g) {
	//Initialize all the things
	game = g;
	Menu::SetGame(game);
	Entity::SetGame(game);
}