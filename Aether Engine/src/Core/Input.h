#pragma once
#include <SDL3/SDL_scancode.h>
#include "../Graphics/Window.h"
#include "../Core/Camera.h"
#include "Vec2.h" //iostream

//Mouse buttons
constexpr int LMB = SDL_BUTTON_LEFT;
constexpr int RMB = SDL_BUTTON_RIGHT;
constexpr int MMB = SDL_BUTTON_MIDDLE;

//Keys
//Letters
constexpr int SCA = SDL_SCANCODE_A;
constexpr int SCB = SDL_SCANCODE_B;
constexpr int SCC = SDL_SCANCODE_C;
constexpr int SCD = SDL_SCANCODE_D;
constexpr int SCG = SDL_SCANCODE_G;
constexpr int SCI = SDL_SCANCODE_I;
constexpr int SCJ = SDL_SCANCODE_J;
constexpr int SCM = SDL_SCANCODE_M;
constexpr int SCO = SDL_SCANCODE_O;
constexpr int SCS = SDL_SCANCODE_S;
constexpr int SCW = SDL_SCANCODE_W;
//Numbers
constexpr int SC0 = SDL_SCANCODE_0;
constexpr int SC1 = SDL_SCANCODE_1;
constexpr int SC2 = SDL_SCANCODE_2;
constexpr int SC3 = SDL_SCANCODE_3;
constexpr int SC4 = SDL_SCANCODE_4;
constexpr int SC5 = SDL_SCANCODE_5;
constexpr int SC6 = SDL_SCANCODE_6;
constexpr int SC7 = SDL_SCANCODE_7;
constexpr int SC8 = SDL_SCANCODE_8;
constexpr int SC9 = SDL_SCANCODE_9;
//Arrows
constexpr int UP = SDL_SCANCODE_UP;
constexpr int DOWN = SDL_SCANCODE_DOWN;
constexpr int LEFT = SDL_SCANCODE_LEFT;
constexpr int RIGHT = SDL_SCANCODE_RIGHT;
//Special
constexpr int LALT = SDL_SCANCODE_LALT;
constexpr int RALT = SDL_SCANCODE_RALT;
constexpr int LCTRL = SDL_SCANCODE_LCTRL;
constexpr int RCTRL = SDL_SCANCODE_RCTRL;
constexpr int LSHIFT = SDL_SCANCODE_LSHIFT;
constexpr int RSHIFT = SDL_SCANCODE_RSHIFT;

namespace Input {

	static Window* window = nullptr;
	static Camera* camera = nullptr;

	//State arrays
	static bool prev_keys[SDL_SCANCODE_COUNT] = { 0 };
	static const bool* curr_keys = nullptr;
	static Uint32 prev_mouse_state = 0;
	static Uint32 curr_mouse_state = 0;
	static Vec2f mouse_pos;
	static uchar res_scale;

	void Init(Window* win, Camera* cam);
	void Update();

	bool BtnDown(int btn);
	bool BtnPressed(int btn);
	bool BtnReleased(int btn);
	Vec2i MousePos();

	bool KeyDown(int key);
	bool KeyPressed(int key);
	bool KeyReleased(int key);
};