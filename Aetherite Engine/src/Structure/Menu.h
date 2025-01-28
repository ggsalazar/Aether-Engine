#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include <vector>
#include <unordered_map>
#include "../Utils/Enums.h"

using namespace std;

//Forward declarations
class Game;
class Scene;
class UI;

class Menu {
public:
    //Variables
    Menus label;
    bool open = false;

    Menu(Game& g, sf::RenderWindow& w, Scene& s, Menus init_label);

    //Game
    void Draw() { if (open) window.draw(menu_text); }

    //UI Elements
    void Resize();
    bool CheckUIElem(const UIElems ui);
    bool GetUIElemActive(const UIElems ui);
    void SetUIElemActive(const UIElems ui, const bool a = true);
    int GetUIElemStatus(const UIElems ui);

private:
    sf::Text menu_text;

    unordered_map<UIElems, shared_ptr<UI>> ui_elems;

    //Game structure pointers
    Game& game;
    sf::RenderWindow& window;
    Scene& scene;
};