#pragma once
#include <unordered_map>
#include "../Engine/Core.h"
#include "../Engine/Graphics/Text.h"

class UI;

class Menu {
public:
    Text menu_text,
        sup_text, //Supplementary text
        tert_text; //Tertiary text
    Sprite sprite;
    unordered_map<Widget, UI*> widgets;
    bool to_close = false, has_focus = false, expired = false;
    int layer_order = 0;

    explicit Menu(const MenuName i_name, Menu* owning_m = nullptr, Entity* owning_e = nullptr);
    ~Menu();
    static inline void SetCore(Core* e, Game* g) { core = e; game = g; }

    //Engine
    void Update();
    void Draw();

    //Self and Sub-Menus
    void Open(const bool o = true);
    inline void SetSupStr(const string& new_str) { sup_text.SetStr(new_str); }
    inline MenuName GetName() const { return name; }
    inline bool GetOpen() const { return open; }
    void OpenSM(const MenuName s_m);

    //Widgets
    void RemoveWidget(const Widget w);
    bool GetWidgetActive(const Widget w);
    void SetWidgetActive(const Widget w, const bool a = true);
    Vec2f GetWidgetPos(const Widget w);
    string GetWidgetStatus(const Widget w);
    void SetWidgetStatus(const Widget w, const string& new_status);

    //Owners
    inline void AssignOwningMenu(Menu* m) { owning_menu = m; }
    void AssignOwningEnt(Entity* e);

protected:
    MenuName name;
    bool open = false;
    Menu* owning_menu = nullptr;
    Entity* owning_ent = nullptr;

    unordered_map<MenuName, Menu*> sub_menus;
    static inline Core* core;
    static inline Game* game;
};
