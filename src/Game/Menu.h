#pragma once
#include <unordered_map>
#include "../Engine/Engine.h"
#include "../Engine/Graphics/Text.h"

class UI;

class Menu {
public:
    bool to_close = false, has_focus = false;

    Menu(const MenuName i_name);
    virtual ~Menu() {
        for (auto& [_, w] : widgets) delete w;
        widgets.clear();

        for (auto& [_, sm] : sub_menus) delete sm;
        sub_menus.clear();
    }
    static inline void SetEngine(Engine* e) { engine = e; }

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
    void RemoveWidget(const Widget ui);
    bool CheckWidget(const Widget ui);
    bool GetWidgetActive(const Widget ui);
    void SetWidgetActive(const Widget ui, const bool a = true);
    void SetWidgetStatus(const Widget ui, const string new_status);
    string GetWidgetStatus(const Widget ui);

protected:
    MenuName name;

    Text menu_text;
    Text sup_text; //Supplementary text
    bool open = false;

    unordered_map<MenuName, Menu*> sub_menus;
    unordered_map<Widget, UI*> widgets;
    Vec2f ui_ori = { .5f, .5f };

    static inline Engine* engine;
};
