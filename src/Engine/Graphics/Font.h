#pragma once
#include <filesystem>
#include <SDL3_ttf/SDL_ttf.h>

class Font {
public:
	Font() = default;
	Font(const std::string& file, uint size) {
		std::string path = "../assets/Fonts/" + file;
		font = TTF_OpenFont((path + ".ttf").c_str(), size);

		if (!font) font = TTF_OpenFont((path+".otf").c_str(), size);

		if (!font)
			std::cout << "Could not load font from file: " << path << "! SDL Error: " << SDL_GetError() << '\n';
	}
	~Font() {
		//This causes crashes for some fucking reason
		//TTF_CloseFont(font);
	}

	[[nodiscard]] inline TTF_Font* GetFont() const { if (font) return font; return nullptr; }

private:
	TTF_Font* font = nullptr;
};