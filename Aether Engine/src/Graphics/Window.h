#pragma once
#define SDL_MAIN_HANDLED
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_video.h>
#include "../Core/Vec2.h" //iostream

class Window {
public:
	bool open = true;
	bool has_focus = true;

	Window(const char* title, Vec2u size = { 0, 0 });
	~Window() {
		SDL_DestroyWindow(window);
	}

	void PollEvents();

	inline SDL_Window* GetWin() const { return window; }

	inline Vec2u ScreenSize() const { return screen_size; }
	inline Vec2u WinSize() const { return win_size; }

private:
	SDL_Window* window;
	SDL_Event event;
	Vec2u screen_size;
	Vec2u win_size;
};