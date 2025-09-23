#pragma once
#include <queue>
#include <unordered_map>
#include "../Engine/Enums.h"

using namespace std;

class Engine;
class Menu;
class Entity;
class UI;

class Game {
public:
    Scene curr_scn = Scene::Title;
    vector<Entity*> entities;

	Game() = default;
    ~Game() {
        for (auto& e : entities) delete e;
        entities.clear();

        for (auto& m : menus) delete m;
        menus.clear();
    }
	void Init(Engine* e);

    void ChangeScene(Scene new_scn);

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

    inline static Engine* engine = nullptr;
};