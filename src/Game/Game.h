#pragma once
#include <vector>
#include "../Engine/Graphics/Sprite.h"
#include "../Engine/Graphics/TileMap.h" //Enums

using namespace std;

class Engine;
class Menu;
class Entity;
class UI;

class Game {
public:
    Scene curr_scn = Scene::Title;
    vector<Entity*> entities;
    bool paused = false;

	Game() = default;
    ~Game();
	void Init(Engine* e);

    //Engine stuff
    void Update();
    void Draw();
    void DrawGUI();
    void Resize();

    //Game Stuff
    void ChangeScene(Scene new_scn);

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
    //Must be a vector to more easily control draw order
    vector<Menu*> menus;
    Sprite cursor;
    TileMap tilemap;

    inline static Engine* engine = nullptr;
};
