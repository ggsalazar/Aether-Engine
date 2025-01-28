#include "Animation.h"
#include "../Entities/Entity.h"

Animation::Animation(Game& g, sf::RenderWindow& w, Entity* e, const AnimInfo& a_i, const Transform& t) :
    game(g), window(w), sprite(texture) {

    //Img/Tex/Spr
    SetSprite(a_i.img_name);

    SetSpriteW(a_i.s_w);
    SetSpriteH(a_i.s_h);
    SetTexRow(a_i.t_r, a_i.n_f); //STR will set the # of frames
    SetCurrFrame(a_i.c_f);
    SetFPS(a_i.fps);

    //Set the origin/anchor, scale, and position
    SetOrigin(t.origin_x, t.origin_y);
    sprite.setScale({ t.scale_w, t.scale_h });
    sprite.setPosition({ t.x, t.y });

    //Entity MUST be initialized at the end due it being referenced in SetSpriteW/H
    entity = e;
}

void Animation::Draw() {
    //Set the current frame in the spritesheet
    tex_rect = sf::IntRect(sf::Vector2i(curr_frame * sprite_w, tex_row * sprite_h), sf::Vector2i(sprite_w, sprite_h));
    sprite.setTextureRect(tex_rect);

    //Go to the next frame in the spritesheet
    if (frame_change_interval != 0 and game.GetFramesElapsed() % frame_change_interval == 0) {
        if (fps > 0)
            SetCurrFrame(curr_frame + 1);
        else
            SetCurrFrame(curr_frame - 1);
    }

    //Draw the sprite
    window.draw(sprite);
}

void Animation::SetSprite(const std::string new_im) {
    if (!img.loadFromFile("assets/Sprites/" + new_im + ".png")) {
        std::cerr << "Failed to load image for " << new_im << std::endl;
        return;
    }
    if (!texture.loadFromImage(img)) {
        std::cerr << "Failed to load texture for " << new_im << std::endl;
        return;
    }
    sprite.setTexture(texture);
}

void Animation::SetSpriteW(const uint new_s_w) {
    if (new_s_w > 0) {
        sf::Vector2f old_ori = GetOrigin();

        sprite_w = new_s_w;
        
        if (entity)
            entity->SetBBox();
        SetOrigin(old_ori.x, old_ori.y);
    }
    else {
        cout << "Sprite width must be > 0!" << endl;
        sprite_w = 1;
    }
}

void Animation::SetSpriteH(const uint new_s_h) {
    if (new_s_h > 0) {
        sf::Vector2f old_ori = GetOrigin();

        sprite_h = new_s_h;

        if (entity)
            entity->SetBBox();
        SetOrigin(old_ori.x, old_ori.y);
    }
    else {
        cout << "Sprite height must be > 0!" << endl;
        sprite_h = 1;
    }
}

void Animation::SetTexRow(int new_t_r, const int new_n_f) {
    //Dividing the size of the image by the height of the sprite should ALWAYS produce a whole number
    const unsigned int num_rows = img.getSize().y / sprite_h;
    while (0 > new_t_r or new_t_r >= num_rows) {
        if (new_t_r < 0)
            new_t_r += num_rows;
        else if (new_t_r >= num_rows)
            new_t_r -= num_rows;
    }
    tex_row = new_t_r;

    //If a new number of frames is not provided, it will be assumed that the # of frames is staying the same
    if (new_n_f != -1)
        SetNumFrames(new_n_f);
}

void Animation::SetCurrFrame(int new_c_f) {
	while (new_c_f < 0 or new_c_f >= num_frames) {
		if (new_c_f < 0)
			new_c_f += num_frames;
		else if (new_c_f >= num_frames)
			new_c_f -= num_frames;
	}
	curr_frame = new_c_f;
}

void Animation::SetFPS(const int new_fps) {
    fps = new_fps;
    if (fps > 0)
        frame_change_interval = game.GetFPS() / fps;
    else if (fps < 0)
        frame_change_interval = game.GetFPS() / (fps * -1);
    frame_change_interval = std::round(frame_change_interval);

    if (fps != 0)
        anim_length = num_frames / fps;
    else anim_length = 0;
}

void Animation::SetOrigin(float x, float y) {
    //Origin/anchor needs to be set based on sprite w/h before scaling is applied - in other words, the literal, actual w/h of the png
    //Use get local bounds?
    if (x > 1.f)
        x = sprite_w * .5;
    else x *= sprite_w;

    if (y > 1.0f)
        y = sprite_h * .5;
    else y *= sprite_h;

    sprite.setOrigin({ x, y });
}