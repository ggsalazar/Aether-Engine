#include "Input.h"

void Input::Init(Window* win) {
	window = win;
	curr_keys = SDL_GetKeyboardState(nullptr);
}

void Input::SetCamera(Camera *cam) {
	camera = cam;
}

void Input::Update() {
	//Keys
	memcpy(prev_keys, curr_keys, SDL_SCANCODE_COUNT);
	curr_keys = SDL_GetKeyboardState(nullptr);

	//Mouse
	prev_mouse_state = curr_mouse_state;
	curr_mouse_state = SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);
}

void Input::UpdateResScale(const int res) {
	res_scale = res / camera->viewport.w;
}

bool Input::BtnDown(const int btn) {
	return window->has_focus and curr_mouse_state & SDL_BUTTON_MASK(btn);
}

bool Input::BtnPressed(const int btn) {
	return window->has_focus and curr_mouse_state & SDL_BUTTON_MASK(btn) and !(prev_mouse_state & SDL_BUTTON_MASK(btn));
}

bool Input::BtnReleased(const int btn) {
	return window->has_focus and !(curr_mouse_state & SDL_BUTTON_MASK(btn)) and prev_mouse_state & SDL_BUTTON_MASK(btn);
}

Vec2f Input::MousePos() {
	return {(mouse_pos.x + camera->viewport.x * res_scale)/res_scale, (mouse_pos.y + camera->viewport.y * res_scale)/res_scale};
}

bool Input::KeyDown(const int key) {
	return window->has_focus and curr_keys[key];
}

bool Input::KeyPressed(const int key) {
	return window->has_focus and curr_keys[key] and !prev_keys[key];
}

bool Input::KeyReleased(const int key) {
	return window->has_focus and !curr_keys[key] and prev_keys[key];
}