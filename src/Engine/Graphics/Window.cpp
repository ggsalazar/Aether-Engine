#include "Window.h"

Window::Window(const char* title, Vec2u size) {

	auto flags = SDL_WINDOW_HIGH_PIXEL_DENSITY;
	if (size == Vec2u{ 0,0 })
		flags = SDL_WINDOW_FULLSCREEN | SDL_WINDOW_HIGH_PIXEL_DENSITY;
	window = SDL_CreateWindow(title, size.x, size.y, flags);

	if (window == nullptr)
		std::cout << "Could not create window: " << SDL_GetError() << "\n";

	int w, h;
	SDL_GetWindowSizeInPixels(window, &w, &h);
	win_size = Vec2u{ (uint)w, (uint)h };

	int display_count = 0;
	SDL_DisplayID* displays = SDL_GetDisplays(&display_count);

	if (displays and display_count) {
		SDL_Rect bounds;

		SDL_GetDisplayBounds(displays[0], &bounds);
		screen_size = { (uint)bounds.w, (uint)bounds.h };

		if (screen_size.x != bounds.w or screen_size.y != bounds.h)
			SDL_Log("Failed to get display bounds: %s", SDL_GetError());
	}
	SDL_free(displays);
}

void Window::PollEvents() {
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_EVENT_WINDOW_FOCUS_GAINED:
			has_focus = true;
			break;
		case SDL_EVENT_WINDOW_FOCUS_LOST:
			has_focus = false;
			break;

		case SDL_EVENT_QUIT:
			open = false;
			break;
		}
	}
}