#include "Menu.h"
#include "../Entities/UI/Button.h"
#include "../Entities/UI/Slider.h"
#include "../Entities/UI/Picker.h"
#include "../Entities/UI/Toggle.h"

Menu::Menu(const MenuName i_name) : name(i_name), menu_text(42), sup_text(30) {

    //Menu and Supp text variables
    menu_text.SetOrigin();
    Vec2i m_t_pos = { 0 };
    string m_t_str = "";

    sup_text.SetOrigin();
    Vec2i s_t_pos = { 0 };
    string s_t_str = "";
    uint s_t_str_max_w = -1;

    Sprite::Info elem_info = {}; elem_info.origin = ui_ori;
    int e_y_buffer = 0;
    //What we do depends on our name
    switch (name) {

    case MenuName::Main: {

        m_t_pos = Round(engine->min_res.x * .5f, engine->min_res.y * .12f); m_t_str = "Aether Engine";
        s_t_pos = Round(m_t_pos.x, engine->min_res.y * .15f); s_t_str = "";

        elem_info.sheet = "UI/Button";
        elem_info.pos = Round(engine->min_res.x * .5f, engine->min_res.y * .4f);

        e_y_buffer = round(engine->min_res.y * .1f);

        elem_info.pos.y += e_y_buffer;
        widgets.insert({ Widget::Options, new Button(elem_info, this, Widget::Options) });

        elem_info.pos.y += e_y_buffer;
        widgets.insert({ Widget::Quit, new Button(elem_info, this, Widget::Quit) });

        break;
    }

    case MenuName::Options: {
        m_t_pos = Round(engine->min_res.x * .5f, engine->min_res.y * .12f); m_t_str = "Aether Engine";
        s_t_pos = Round(m_t_pos.x, engine->min_res.y * .15f); s_t_str = "Options";

        elem_info.pos = Round(m_t_pos.x, engine->min_res.y * .3f);
        elem_info.sheet = "UI/Slider";

        e_y_buffer = round(engine->min_res.y * .09f);

        //Music and sfx sliders
        widgets.insert({ Widget::Music_V, new Slider(elem_info, this, Widget::Music_V) });

        elem_info.pos.y += e_y_buffer;
        widgets.insert({ Widget::SFX_V, new Slider(elem_info, this, Widget::SFX_V) });


        //Resolution picker, fullscreen toggle, and apply button
        elem_info.sheet = "UI/Button";
        elem_info.pos.y += e_y_buffer;
        widgets.insert({ Widget::Resolution, new Picker(elem_info, this, Widget::Resolution) });


        elem_info.sheet = "UI/Toggle"; elem_info.frame_size = { 24 };
        elem_info.pos.y += e_y_buffer;
        elem_info.pos.x += 34;
        widgets.insert({ Widget::Fullscreen, new Toggle(elem_info, this, Widget::Fullscreen) });
        elem_info.pos.x -= 34;

        //Reset the size of a single frame
        elem_info.frame_size = { 0 };

        elem_info.sheet = "UI/Button";
        elem_info.pos.y += e_y_buffer;
        widgets.insert({ Widget::Apply, new Button(elem_info, this, Widget::Apply) });


        //Back button
        elem_info.pos.y = round(engine->min_res.y * .9f);
        widgets.insert({ Widget::Back, new Button(elem_info, this, Widget::Back) });

        break;
    }
    }


    //Set our texts (not strictly necessary but keeping for now)
    menu_text.MoveTo(m_t_pos); menu_text.SetStr(m_t_str); menu_text.SetMaxW(engine->min_res.x);
    sup_text.MoveTo(s_t_pos); sup_text.SetStr(s_t_str); sup_text.SetOrigin({ .5f, .0f }); sup_text.SetMaxW(engine->min_res.x);
    if (s_t_str_max_w != -1)
        sup_text.SetMaxW(s_t_str_max_w);
}


void Menu::GetInput() {
    if (open) {
        for (auto& w : widgets)
            w.second->GetInput();

        for (auto& s_m : sub_menus)
            s_m.second->GetInput();
    }
}

void Menu::Update() {
    if (open) {
    	for (auto& w : widgets) w.second->Update();

        for (const auto& s_m : sub_menus)
            s_m.second->Update();
    }
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

    for (auto& w : widgets) {
        w.second->label.SetFont();
        if (Picker* p = dynamic_cast<Picker*>(w.second)) p->SetPickingF();
        else if (Slider* s = dynamic_cast<Slider*>(w.second)) s->SetKLF();
    }
}

void Menu::Open(const bool o) {
    open = o;
    has_focus = open;

    if (!open) {
        for (const auto& sm : sub_menus)
            sm.second->Open(false);
    }
}

void Menu::OpenSM(const MenuName s_m) {
    if (sub_menus.count(s_m) > 0)
        sub_menus[s_m]->Open();
    else cout << "That Sub-Menu does not exist in this Menu!\n";
}

void Menu::RemoveWidget(const Widget w) {
    if (CheckWidget(w)) {
        //delete widgets[w];
        widgets.erase(w);
    }
}

bool Menu::CheckWidget(const Widget w) {
    if (widgets.find(w) != widgets.end()) return true;

    cout << "That Widget does not exist!" << endl;
    return false;
}

void Menu::SetWidgetStatus(const Widget w, const string new_status) {

    if (CheckWidget(w)) {
        if (auto picker = dynamic_cast<Picker*>(widgets[w]))
            picker->SetPicking(new_status);
        else if (auto toggle = dynamic_cast<Toggle*>(widgets[w]))
        	toggle->on = new_status == "True";
    }
}

void Menu::SetWidgetActive(const Widget w, const bool a) {
    if (CheckWidget(w))
        widgets[w]->SetActive(a);
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

    return "No such Widget exists";
}

bool Menu::GetWidgetActive(const Widget w) {
    if (CheckWidget(w))
        return widgets[w]->GetActive();

    return false;
}
