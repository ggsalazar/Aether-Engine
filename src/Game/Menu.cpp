#include "Menu.h"
#include "../Entities/UI/Button.h"
#include "../Entities/UI/Slider.h"
#include "../Entities/UI/Picker.h"
#include "../Entities/UI/Toggle.h"

Menu::Menu(const MenuName i_name, Menu* owning_m, Entity* owning_e)
    : name(i_name), owning_menu(owning_m), owning_ent(owning_e),
      menu_text(FontName::Normal, 40), sup_text(FontName::Normal, 20), tert_text(FontName::Normal, 10) {

    if (owning_menu) layer_order = owning_menu->layer_order+3;

    //Sprite
    Sprite::Info info = {};
    info.origin = { .5f };
    info.pos = gm->camera.GetCenter();
    info.default_layer = LayerName::UI;
    info.layer_order = layer_order;

    //Menu/Sup/Ter Text
    menu_text.SetOrigin(); menu_text.SetLayerOrder(layer_order+1); menu_text.SetMaxW(core->min_res.x);
    sup_text.SetOrigin(); sup_text.SetLayerOrder(layer_order+1); sup_text.SetMaxW(core->min_res.x); sup_text.SetOrigin({ .5f, .0f });
    tert_text.SetMaxW(334); tert_text.SetLayerOrder(layer_order+1);

    Vec2f elem_pos = info.pos;
    float e_y_buffer = 0;

    //What we do depends on our name
    switch (name) {
        case MenuName::Main: {
            menu_text.MoveTo({core->min_res.x * .5f, core->min_res.y * .12f});
            menu_text.SetStr("Aether Engine");
            sup_text.MoveTo({menu_text.GetPos().x, core->min_res.y * .15f});

            elem_pos = {core->min_res.x * .5f, core->min_res.y * .4f};
            e_y_buffer = core->min_res.y * .1f;

            widgets.insert({ Widget::Options, new Button(elem_pos, this, Widget::Options) });

            elem_pos.y += e_y_buffer;
            widgets.insert({ Widget::Quit, new Button(elem_pos, this, Widget::Quit) });

            break;
        }

        case MenuName::Options: {
            menu_text.MoveTo({core->min_res.x * .5f, core->min_res.y * .12f});
            menu_text.SetStr("Aether Engine");
            sup_text.MoveTo({menu_text.GetPos().x, core->min_res.y * .15f});
            sup_text.SetStr("Options");

            elem_pos = {menu_text.GetPos().x, core->min_res.y * .3f};
            e_y_buffer = core->min_res.y * .09f;

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
            elem_pos.y = core->min_res.y * .9f;
            widgets.insert({ Widget::Back, new Button(elem_pos, this, Widget::Back) });

            break;
        }
    }
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
        sprite.Draw();
        menu_text.Draw();
        sup_text.Draw();
        tert_text.Draw();

        for (auto& w : widgets)
            w.second->Draw();

        for (auto& s_m : sub_menus)
            s_m.second->Draw();
    }
}

void Menu::Open(const bool o) {
    open = o;
    has_focus = open;
    to_close = !open;

    if (!open) {
        if (owning_menu) {
            owning_menu->has_focus = true;
            expired = true;
        }

        tert_text.SetStr("");

        for (const auto& sm : sub_menus)
            sm.second->Open(false);
    }
}

void Menu::OpenSM(const MenuName s_m) {
    if (sub_menus.count(s_m) > 0)
        sub_menus[s_m]->Open();
    else cerr << "Menu::OpenSM(): That Sub-Menu does not exist in this Menu!\n";
}

void Menu::RemoveWidget(const Widget w) {
    if (widgets.contains(w)) {
        delete widgets[w];
        widgets.erase(w);
    }
}

bool Menu::GetWidgetActive(const Widget w) {
    if (widgets.contains(w))
        return widgets[w]->GetActive();

    cerr << "Menu::GetWidgetActive(): No such Widget exists\n";
    return false;
}

void Menu::SetWidgetActive(const Widget w, const bool a) {
    if (widgets.contains(w))
        widgets[w]->SetActive(a);
}

Vec2f Menu::GetWidgetPos(const Widget w) {
    if (widgets.contains(w))
        return widgets[w]->GetPos();

    cout << "Menu::GetWidgetPos(): No such Widget exists\n";
    return {};
}

string Menu::GetWidgetStatus(const Widget w) {
    if (widgets.contains(w)) {
        if (auto picker = dynamic_cast<Picker*>(widgets[w]))
            return picker->GetPicking();
        if (auto toggle = dynamic_cast<Toggle*>(widgets[w])) {
            if (toggle->on) return "True";
            return "False";
        }
    }

    return "";
}

void Menu::SetWidgetStatus(const Widget w, const string& new_status) {
    if (widgets.contains(w)) {
        if (auto picker = dynamic_cast<Picker*>(widgets[w]))
            picker->SetPicking(new_status);
        else if (auto toggle = dynamic_cast<Toggle*>(widgets[w]))
        	toggle->on = new_status == "True";
    }
}