#include "Menu.h"
#include "../Entities/UI/Button.h"
#include "../Entities/UI/Slider.h"
#include "../Entities/UI/Picker.h"
#include "../Entities/UI/Toggle.h"

Menu::Menu(const MenuName i_name) : name(i_name), menu_text(42), sup_text(30) {

    //Menu and Supp text variables
    menu_text.SetOrigin();
    Vec2f m_t_pos = { 0 };
    string m_t_str;

    sup_text.SetOrigin();
    Vec2f s_t_pos = { 0 };
    string s_t_str;
    uint s_t_str_max_w = 0;

    Vec2f elem_pos;
    float e_y_buffer = 0;
    //What we do depends on our name
    switch (name) {
        case MenuName::Main: {
            m_t_pos = {engine->min_res.x * .5f, engine->min_res.y * .12f}; m_t_str = "Aether Engine";
            s_t_pos = {m_t_pos.x, engine->min_res.y * .15f}; s_t_str = "";

            elem_pos = {engine->min_res.x * .5f, engine->min_res.y * .4f};
            e_y_buffer = engine->min_res.y * .1f;

            widgets.insert({ Widget::Options, new Button(elem_pos, this, Widget::Options) });

            elem_pos.y += e_y_buffer;
            widgets.insert({ Widget::Quit, new Button(elem_pos, this, Widget::Quit) });

            break;
        }

        case MenuName::Options: {
            m_t_pos = {engine->min_res.x * .5f, engine->min_res.y * .12f}; m_t_str = "Aether Engine";
            s_t_pos = {m_t_pos.x, engine->min_res.y * .15f}; s_t_str = "Options";

            elem_pos = {m_t_pos.x, engine->min_res.y * .3f};
            e_y_buffer = engine->min_res.y * .09f;

            //Music and sfx sliders
            widgets.insert({ Widget::Msc_V, new Slider(elem_pos, this, Widget::Msc_V) });

            elem_pos.y += e_y_buffer;
            widgets.insert({ Widget::SFX_V, new Slider(elem_pos, this, Widget::SFX_V) });


            //Resolution picker, fullscreen toggle, and apply button
            elem_pos.y += e_y_buffer;
            widgets.insert({ Widget::Resolution, new Picker(elem_pos, this, Widget::Resolution) });

            elem_pos.y += e_y_buffer;
            elem_pos.x += 34;
            widgets.insert({ Widget::Fullscreen, new Toggle(elem_pos, this, Widget::Fullscreen) });
            elem_pos.x -= 34;

            elem_pos.y += e_y_buffer;
            widgets.insert({ Widget::Apply, new Button(elem_pos, this, Widget::Apply) });

            //Back button
            elem_pos.y = engine->min_res.y * .9f;
            widgets.insert({ Widget::Back, new Button(elem_pos, this, Widget::Back) });

            break;
        }
    }


    //Set our texts (not strictly necessary but keeping for now)
    menu_text.MoveTo(m_t_pos); menu_text.SetStr(m_t_str); menu_text.SetMaxW(engine->min_res.x);
    sup_text.MoveTo(s_t_pos); sup_text.SetStr(s_t_str); sup_text.SetOrigin({ .5f, .0f }); sup_text.SetMaxW(engine->min_res.x);
    if (s_t_str_max_w != 0)
        sup_text.SetMaxW(s_t_str_max_w);
}

Menu::~Menu() {
    for (auto& [_, w] : widgets) delete w;
    widgets.clear();

    for (auto& [_, sm] : sub_menus) delete sm;
    sub_menus.clear();
}

void Menu::Update() {
    if (open) {
    	for (auto& w : widgets) w.second->Update();

        for (const auto& s_m : sub_menus)
            s_m.second->Update();
    }
    if (to_close) Open(false);
}

void Menu::Draw() {
    if (open) {
        engine->renderer.DrawTxt(menu_text);
        engine->renderer.DrawTxt(sup_text);

        for (auto& w : widgets)
            w.second->Draw();

        for (auto& s_m : sub_menus)
            s_m.second->Draw();
    }
}

void Menu::Resize() {
    //Have to resize all the fonts
    menu_text.SetFont();
    sup_text.SetFont();

    for (auto& [_, w] : widgets) {
        w->label.SetFont();
        if (Picker* p = dynamic_cast<Picker*>(w)) p->SetPickingF();
        else if (Slider* s = dynamic_cast<Slider*>(w)) s->SetKLF();
    }
}

void Menu::Open(const bool o) {
    open = o;
    has_focus = open;

    if (!open)
        for (const auto& sm : sub_menus)
            sm.second->Open(false);
}

void Menu::OpenSM(const MenuName s_m) {
    if (sub_menus.count(s_m) > 0)
        sub_menus[s_m]->Open();
    else cout << "Menu::OpenSM(): That Sub-Menu does not exist in this Menu!\n";
}

void Menu::RemoveWidget(const Widget w) {
    if (CheckWidget(w)) {
        delete widgets[w];
        widgets.erase(w);
    }
}

bool Menu::CheckWidget(const Widget w) {
    if (widgets.find(w) != widgets.end()) return true;

    cout << "Menu::CheckWidget(): That Widget does not exist!" << endl;
    return false;
}

bool Menu::GetWidgetActive(const Widget w) {
    if (CheckWidget(w))
        return widgets[w]->GetActive();

    return false;
}

void Menu::SetWidgetActive(const Widget w, const bool a) {
    if (CheckWidget(w))
        widgets[w]->SetActive(a);
}

Vec2i Menu::GetWidgetPos(const Widget w) {
    if (CheckWidget(w))
        return widgets[w]->GetPos();

    cout << "Menu::GetWidgetPos(): No such Widget exists\n";
    return {};
}

string Menu::GetWidgetStatus(const Widget w) {
    if (CheckWidget(w)) {
        if (auto picker = dynamic_cast<Picker*>(widgets[w]))
            return picker->GetPicking();
        else if (auto toggle = dynamic_cast<Toggle*>(widgets[w])) {
            if (toggle->on) return "True";
            return "False";
        }
    }

    return "";
}

void Menu::SetWidgetStatus(const Widget w, const string& new_status) {
    if (CheckWidget(w)) {
        if (auto picker = dynamic_cast<Picker*>(widgets[w]))
            picker->SetPicking(new_status);
        else if (auto toggle = dynamic_cast<Toggle*>(widgets[w]))
        	toggle->on = new_status == "True";
    }
}