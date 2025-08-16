#pragma once
#include <filesystem>
#include <SDL3/SDL_render.h>
#include <SDL3_ttf/SDL_ttf.h>

class Font {
public:
	Font() = default;
	Font(const std::string& file, uint size) {
		std::string path = "assets/Fonts/" + file + ".ttf";
		font = TTF_OpenFont(path.c_str(), size);

		if (!font) {
			path = "assets/Fonts/" + file + ".otf";
			font = TTF_OpenFont(path.c_str(), size);
		}

		if (!font)
			std::cout << "Could not load font from file: " << path << "! SDL Error: " << SDL_GetError() << "\n";

	}
	~Font() {
		//TTF_CloseFont(font);
	}

	inline TTF_Font* GetFont() const { if (font) return font; return nullptr; }

private:
	TTF_Font* font = nullptr;
};