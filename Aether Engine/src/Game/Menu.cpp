#include "Menu.h"
#include "../Entities/UI/Button.h"
#include "../Entities/UI/Slider.h"
#include "../Entities/UI/Picker.h"
#include "../Entities/UI/Toggle.h"

Menu::Menu(const MenuName i_name) : name(i_name) {

    //Font sizes will have to be determined based on the resolution scale
    menu_text.Init(&game->default_fonts[42 * Text::res_scale]);
    sup_text.Init(&game->default_fonts[30 * Text::res_scale]);

    //Menu and Supp text variables
    menu_text.SetOrigin();
    //Text has to be placed at the *actual* resolution
    Vec2i m_t_pos = { 0 };
    string m_t_str = "MENU DEFAULT";

    sup_text.SetOrigin();
    Vec2i s_t_pos = { 0 };
    string s_t_str = "SUPPLEMENTARY DEFAULT";
    uint s_t_str_max_w = -1;

    Sprite::Info elem_info = {}; elem_info.origin = ui_ori;
    int e_y_buffer = 0;
    //What we do depends on our name
    switch (name) {

    case MenuName::Main: {

        m_t_pos = Round(game->min_res.x * .5f, game->min_res.y * .12f); m_t_str = "Shadows of Autongrad";
        s_t_pos = Round(m_t_pos.x, game->min_res.y * .15f); s_t_str = "An Iron & Aether Adventure";

        elem_info.sheet = "UI/Button";
        elem_info.pos = Round(game->min_res.x * .5f, game->min_res.y * .4f);

        e_y_buffer = round(game->min_res.y * .1f);

        elem_info.pos.y += e_y_buffer;
        ui_elems.insert({ UIElem::Load, new Button(elem_info, this, UIElem::Load) });

        elem_info.pos.y += e_y_buffer;
        ui_elems.insert({ UIElem::Options, new Button(elem_info, this, UIElem::Options) });

        elem_info.pos.y += e_y_buffer;
        ui_elems.insert({ UIElem::Quit, new Button(elem_info, this, UIElem::Quit) });

        break;
    }

    case MenuName::Options: {
        m_t_pos = Round(game->min_res.x * .5f, game->min_res.y * .12f); m_t_str = "Shadows of Autongrad";
        s_t_pos = Round(m_t_pos.x, game->min_res.y * .15f); s_t_str = "Options";

        elem_info.pos = Round(m_t_pos.x, game->min_res.y * .3f);
        elem_info.sheet = "UI/Slider";

        e_y_buffer = round(game->min_res.y * .09f);

        //Music and sfx sliders
        ui_elems.insert({ UIElem::Music_V, new Slider(elem_info, this, UIElem::Music_V) });

        elem_info.pos.y += e_y_buffer;
        ui_elems.insert({ UIElem::SFX_V, new Slider(elem_info, this, UIElem::SFX_V) });


        //Resolution picker, fullscreen toggle, and apply button
        elem_info.sheet = "UI/Button";
        elem_info.pos.y += e_y_buffer;
        ui_elems.insert({ UIElem::Resolution, new Picker(elem_info, this, UIElem::Resolution) });


        elem_info.sheet = "UI/Toggle"; elem_info.frame_size = { 24 };
        elem_info.pos.y += e_y_buffer;
        elem_info.pos.x += 34;
        ui_elems.insert({ UIElem::Fullscreen, new Toggle(elem_info, this, UIElem::Fullscreen) });
        elem_info.pos.x -= 34;

        //Reset the size of a single frame
        elem_info.frame_size = { 0 };

        elem_info.sheet = "UI/Button";
        elem_info.pos.y += e_y_buffer;
        ui_elems.insert({ UIElem::Apply, new Button(elem_info, this, UIElem::Apply) });


        //Back button
        elem_info.pos.y = round(game->min_res.y * .9f);
        ui_elems.insert({ UIElem::Back, new Button(elem_info, this, UIElem::Back) });

        break;
    }
    }


    //Set our texts (not strictly necessary but keeping for now)
    menu_text.MoveTo(m_t_pos); menu_text.SetStr(m_t_str); menu_text.SetLogicalMaxW(game->min_res.x);
    sup_text.MoveTo(s_t_pos); sup_text.SetStr(s_t_str); sup_text.SetOrigin({ .5f, .0f }); sup_text.SetLogicalMaxW(game->min_res.x);
    if (s_t_str_max_w != -1)
        sup_text.SetLogicalMaxW(s_t_str_max_w);
}


void Menu::GetInput() {
    if (open) {
        for (auto& uie : ui_elems) {
            uie.second->GetInput();
            // cout << uie.second->GetPos() << '\n';
        }

        for (auto& s_m : sub_menus)
            s_m.second->GetInput();
    }
}

void Menu::Update() {
    //Should this also be updating ui_elems? Otherwise this ain't doing shit
    if (open) {
        for (const auto& s_m : sub_menus)
            s_m.second->Update();
    }
}

void Menu::Draw() {
    if (open) {
        game->renderer.DrawTxt(menu_text);
        game->renderer.DrawTxt(sup_text);

        for (auto& uie : ui_elems)
            uie.second->Draw();

        for (auto& s_m : sub_menus)
            s_m.second->Draw();
    }
}

void Menu::Resize() {
    //Have to resize all the fonts
    menu_text.font = &game->default_fonts[42 * Text::res_scale];
    sup_text.font = &game->default_fonts[30 * Text::res_scale];

    for (auto& uie : ui_elems) {
        uie.second->label.font = &game->default_fonts[18 * Text::res_scale];
        if (Picker* p = dynamic_cast<Picker*>(uie.second)) p->SetPickingF();
        else if (Slider* s = dynamic_cast<Slider*>(uie.second)) s->SetKLF();
    }
}

void Menu::Open(const bool o) {
    open = o;

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

void Menu::RemoveUIElem(const UIElem ui) {
    if (CheckUIElem(ui)) {
        delete ui_elems[ui];
        ui_elems.erase(ui);
    }
}

bool Menu::CheckUIElem(const UIElem ui) {
    if (ui_elems.find(ui) != ui_elems.end()) return true;

    cout << "That UI Element does not exist!" << endl;
    return false;
}

void Menu::SetUIElemStatus(const UIElem ui, const string new_status) {

    if (CheckUIElem(ui)) {
        if (auto picker = dynamic_cast<Picker*>(ui_elems[ui]))
            picker->SetPicking(new_status);
        else if (auto toggle = dynamic_cast<Toggle*>(ui_elems[ui])) {
            if (new_status == "True")
                toggle->on = true;
            else
                toggle->on = false;
        }
    }
}

void Menu::SetUIElemActive(const UIElem ui, const bool a) {
    if (CheckUIElem(ui))
        ui_elems[ui]->SetActive(a);
}

string Menu::GetUIElemStatus(const UIElem ui) {

    if (CheckUIElem(ui)) {
        if (auto picker = dynamic_cast<Picker*>(ui_elems[ui]))
            return picker->GetPicking();
        else if (auto toggle = dynamic_cast<Toggle*>(ui_elems[ui])) {
            if (toggle->on) return "True";
            return "False";
        }
    }

    return "No such UIElem exists";
}

bool Menu::GetUIElemActive(const UIElem ui) {
    if (CheckUIElem(ui))
        return ui_elems[ui]->GetActive();

    return false;
}
