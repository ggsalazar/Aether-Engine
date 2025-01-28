#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include <vector>
#include "../Utils/Enums.h"
#include "Menu.h"

//Forward declarations
class Game;
class Entity;

class Scene {
public:
    Scenes label;

    Scene(Game& g, sf::RenderWindow& win, Scenes init_label) :
        game(g), window(win), label(init_label) {}

    void GetInput();
    void Update();
    void Draw();

    void Open();
    void Close();
    bool IsOpen() const { return open; }
    
    void OpenMenu(Menus menu);
    bool MenuOpen(Menus menu);
    void CloseMenu(Menus menu);
    void ResizeMenus();
    
    void AddEntity(shared_ptr<Entity> e) { entities.push_back(e); }

private:
    bool open = false;

    unordered_map<Menus, unique_ptr<Menu>> menus;
    vector<shared_ptr<Entity>> entities;

    //Game structure stuff
    Game& game;
    sf::RenderWindow& window;
};