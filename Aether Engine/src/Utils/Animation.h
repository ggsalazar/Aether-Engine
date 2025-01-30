 #pragma once
#include <SFML/Graphics.hpp>
#include "../Structure/Game.h"
#include "Util.h"

class Game;
class Entity;

struct AnimInfo {
	std::string img_name;
	uint sprite_w = 1;
	uint sprite_h = 1;
	uint tex_row = 0;
	uint num_frames = 1;
	uint curr_frame = 0;
	int fps = 0;
	uint fci = 0; //frame_change_interval: How many frames until the next frame of the animation
	float anim_length = 0.f; //Length of the animation in seconds
};

class Animation {
	friend class Entity;
public:
	//Position and scale
	struct Transform {
		float x = .0f;
		float y = .0f;
		float origin_x = .0f;
		float origin_y = .0f;
		float scale_w = 1.f;
		float scale_h = scale_w;
	};


	Animation(Game& g, sf::RenderWindow& w, Entity* e, const AnimInfo& a_i, const Transform& t = {});


	void Draw();

	void SetSprite(const std::string new_im);

	void SetSpriteW(const uint new_s_w);
	uint GetSpriteW() const { return anim_info.sprite_w; }

	void SetSpriteH(const uint new_s_h);
	uint GetSpriteH() const { return anim_info.sprite_h; }

	void SetTexRow(int new_t_r, const int new_n_f = -1);
	uint GetTexRow() const { return anim_info.tex_row; }

	void SetCurrFrame(int new_c_f);
	uint GetCurrFrame() const { return anim_info.curr_frame; }

	void SetNumFrames(const int new_n_f) { if (new_n_f > 0) anim_info.num_frames = new_n_f; }
	uint GetNumFrames() const { return anim_info.num_frames; }

	void SetFPS(const int new_fps);
	uint GetFPS() const { return anim_info.fps; }


	void SetOrigin(float x = .5f, float y = .5f);
	//Returns origin from 0-1
	sf::Vector2f GetOrigin() { return sf::Vector2f(sprite.getOrigin().x/ anim_info.sprite_w, sprite.getOrigin().y/ anim_info.sprite_h); }

private:
	AnimInfo anim_info;

	sf::Image img;
	sf::Texture texture;
	sf::IntRect tex_rect;
	sf::Sprite sprite;

	Game& game;
	sf::RenderWindow& window;
	Entity* entity;
};