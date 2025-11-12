#pragma once
#include <unordered_map>
#include "../Engine/Engine.h"
#include "../Engine/Graphics/Text.h"

class UI;

class Menu {
public:
    bool to_close = false, has_focus = false;

    explicit Menu(const MenuName i_name);
    ~Menu();
    static inline void SetEngine(Engine* e) { engine = e; }

    //Engine
    void Update();
    void Draw();

    void Resize();

    //Self and Sub-Menus
    void Open(const bool o = true);
    inline void SetSupStr(const string& new_str) { sup_text.SetStr(new_str); }
    inline MenuName GetName() const { return name; }
    inline bool GetOpen() const { return open; }
    void OpenSM(const MenuName s_m);

    //Widgets
    bool CheckWidget(const Widget w);
    void RemoveWidget(const Widget w);
    bool GetWidgetActive(const Widget w);
    void SetWidgetActive(const Widget w, const bool a = true);
    Vec2i GetWidgetPos(const Widget w);
    string GetWidgetStatus(const Widget w);
    void SetWidgetStatus(const Widget w, const string& new_status);

protected:
    MenuName name;

    Text menu_text;
    Text sup_text; //Supplementary text
    bool open = false;

    unordered_map<MenuName, Menu*> sub_menus;
    unordered_map<Widget, UI*> widgets;

    static inline Engine* engine;
};
