#pragma once
#include <unordered_map>
#include "Game.h"

class UI;

class Menu {
public:
    bool to_close = false;

    Menu(const MenuName i_name);
    virtual ~Menu() {
        for (auto& [_, ui] : ui_elems) {
            delete ui;
            ui = nullptr;
        }
        ui_elems.clear();

        for (auto& [_, sm] : sub_menus) {
            delete sm;
            sm = nullptr;
        }
        sub_menus.clear();
    }
    static inline void SetGame(Game* g) { game = g; }

    //Engine
    virtual void GetInput();
    virtual void Update();
    virtual void Draw();

    void Resize();

    //Self and Sub-Menus
    void Open(const bool o = true);
    inline void SetSupStr(const string& new_str) { sup_text.SetStr(new_str); }
    inline MenuName GetName() const { return name; }
    inline bool GetOpen() const { return open; }
    void OpenSM(const MenuName s_m);

    //UI Elements
    void RemoveUIElem(const UIElem ui);
    bool CheckUIElem(const UIElem ui);
    bool GetUIElemActive(const UIElem ui);
    void SetUIElemActive(const UIElem ui, const bool a = true);
    void SetUIElemStatus(const UIElem ui, const string new_status);
    string GetUIElemStatus(const UIElem ui);

protected:
    MenuName name;

    Text menu_text;
    Text sup_text; //Supplementary text
    bool open = false;

    unordered_map<MenuName, Menu*> sub_menus;
    unordered_map<UIElem, UI*> ui_elems;
    Vec2f ui_ori = { .5f, .5f };

    static inline Game* game;
};