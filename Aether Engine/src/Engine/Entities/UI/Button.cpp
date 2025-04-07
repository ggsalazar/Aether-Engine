#include "Button.h"

void Button::Draw(const bool debug) {
    if (active and Selected(MOUSEPOS))
        window.draw(bbox_debug);

    Entity::Draw(debug);
    window.draw(label);
}

void Button::Move() {
    Entity::Move();
    label_offset = game.GetResScale();
    label.setPosition({ pos.x, pos.y - label_offset });
}

void Button::Pressed() {
    UI::Pressed();
}

void Button::Released() {
    switch (elem) {
        case UIElems::APPLY:
            if (menu.label == Menus::OPTIONS) {
                //Set the game's current resolution to the scale determined by the resolution picker OR set it to fullscreen if that toggle is clicked
                bool flscrn = menu.GetUIElemStatus(UIElems::FULLSCREEN);
                if (flscrn)
                    game.SetResolution(Vector2u(SCREENW(), SCREENH()));
                else {
                    uint new_scale = menu.GetUIElemStatus(UIElems::RESOLUTION);
                    uint old_scale = game.GetResScale();
                    if (new_scale != old_scale)
                        game.SetResolution(new_scale);
                }
                SetActive(false);
            }
        break;

        case UIElems::BACK:
            //Close current menu
            menu.open = false;

            //Re-open the MM
            scene.OpenMenu(Menus::MAIN);
        break;

        case UIElems::LEVSEL:
            scene.OpenMenu(Menus::LEVSEL);
            menu.open = false;
        break;

        case UIElems::OPTIONS:
            scene.OpenMenu(Menus::OPTIONS);
            menu.open = false;
        break;

        case UIElems::LEV1:
            game.level = 1;
            game.game_scene->AddEntity(shared_from_this());
            //Switch active scene to game_scene (handles closing menus and deleting entities)
            game.SetScene(Scenes::GAME);
        break;

        case UIElems::LEV2:
            game.level = 2;
            game.game_scene->AddEntity(shared_from_this());
            //Switch active scene to game_scene (handles closing menus and deleting entities)
            game.SetScene(Scenes::GAME);
        break;

        case UIElems::LEV3:
            game.level = 3;
            game.game_scene->AddEntity(shared_from_this());
            //Switch active scene to game_scene (handles closing menus and deleting entities)
            game.SetScene(Scenes::GAME);
        break;

        case UIElems::MAINMENU:
            game.paused = false;
            game.game_over = false;
            game.title_scene->AddEntity(shared_from_this());
            game.SetScene(Scenes::TITLE);
        break;

        case UIElems::QUIT:
            window.close();
        break;

        case UIElems::RESUME:
            game.paused = false;
            menu.open = false;
        break;
    }
}
