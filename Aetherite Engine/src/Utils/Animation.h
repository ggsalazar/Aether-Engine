 #pragma once
#include <SFML/Graphics.hpp>
#include "../Structure/Game.h"
#include "Util.h"

class Game;
class Entity;

class Animation {
	friend class Entity;
public:
	//Anim Info
	struct AnimInfo {
		std::string img_name;
		uint s_w = 1;
		uint s_h = 1;
		uint t_r = 0;
		uint n_f = 1;
		uint c_f = 0;
		int fps = 0;
	};

	//Position and scale
	struct Transform {
		float x = .0f;
		float y = .0f;
		float origin_x = .0f;
		float origin_y = .0f;
		float scale_w = 1.f;
		float scale_h = scale_w;
	};

	Game& game;
	sf::RenderWindow& window;
	Entity* entity;

	sf::Sprite sprite;

	Animation(Game& g, sf::RenderWindow& w, Entity* e, const AnimInfo& a_i, const Transform& t = {});


	void Draw();

	void SetSprite(const std::string new_im);

	void SetSpriteW(const uint new_s_w);
	uint GetSpriteW() const { return sprite_w; }

	void SetSpriteH(const uint new_s_h);
	uint GetSpriteH() const { return sprite_h; }

	void SetTexRow(int new_t_r, const int new_n_f = -1);
	uint GetTexRow() const { return tex_row; }

	void SetCurrFrame(int new_c_f);
	uint GetCurrFrame() const { return curr_frame; }

	void SetNumFrames(const int new_n_f) { if (new_n_f > 0) num_frames = new_n_f; }
	uint GetNumFrames() const { return num_frames; }

	void SetFPS(const int new_fps);
	uint GetFPS() const { return fps; }


	void SetOrigin(float x = .5f, float y = .5f);
	//Returns origin from 0-1
	sf::Vector2f GetOrigin() { return sf::Vector2f(sprite.getOrigin().x/sprite_w, sprite.getOrigin().y/sprite_h); }

private:
	//Variables
	uint sprite_w = 1;
	uint sprite_h = 1;
	uint tex_row = 0;
	uint num_frames = 1;
	uint curr_frame = 0;
	int fps = 0; //How many frames of the animation play in a second
	uint frame_change_interval = 0; //How many frames until the next frame of the animation
	float anim_length = 0.f; //Length of the animation in seconds

	sf::Image img;
	sf::Texture texture;
	sf::IntRect tex_rect;
};