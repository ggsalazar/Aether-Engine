#pragma once
#include <queue>
#include <unordered_map>
#include "../Core/Enums.h"

using namespace std;

class Game;
class Menu;
class Entity;
class UI;

class Scene {
public:
    vector<Entity*> entities;

	Scene() = default;
	~Scene();
	void Init(Game* g);

    //Engine stuff
    void GetInput();
    void Update();
    void Draw();
    void DrawGUI();
    void Resize();

    //Menu handling
    void CreateMenu(const MenuName menu);
    void OpenMenu(const MenuName menu, const bool o = true);
    bool MenuOpen(const MenuName menu);
    Menu* FindMenu(const MenuName menu);

    //Entities
    inline void AddEntity(Entity* e) { entities.push_back(e); }
    void RemoveEntity(Entity* e);
    void SetEntitySFXVolume(const float new_volume);

private:
    vector<Menu*> menus;

    inline static Game* game = nullptr;
};