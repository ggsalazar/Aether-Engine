#pragma once
#include <vector>
#include "../Engine/Audio/DJ.h"
#include "../Engine/Graphics/Camera.h"
#include "../Engine/Graphics/TileMap.h" //Enums

using namespace std;

class Core;
class Entity;
class Menu;
class UI;

class Game {
public:
    Camera camera;
    DJ dj;
    Scene curr_scn = Scene::NONE;
    vector<Entity*> entities;
    bool paused = false;

	Game() = default;
    ~Game();
	void Init(Core* e);

    //Engine stuff
    void Update();
    void Draw();
    void DrawGUI();

    //Game Stuff
    void ChangeScene(Scene new_scn);

    //Menu handling
    void CreateMenu(const MenuName menu, Menu* menu_owner = nullptr);
    void OpenMenu(const MenuName menu);

private:
    //Must be a vector to more easily control draw order
    unordered_map<MenuName, Menu*> menus;
    Sprite cursor;
    TileMap tilemap;

    inline static Core* core = nullptr;
};
