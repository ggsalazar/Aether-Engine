#pragma once
#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <vector>
#include "../Math/Geometry.h"

class Camera;
class Sprite;
class Text;

class Renderer {
public:
	Renderer(SDL_Window* window, Camera* cam);
	~Renderer() {
		if (renderer) SDL_DestroyRenderer(renderer);
		if (surface) SDL_DestroySurface(surface);
		if (text_tar) SDL_DestroyTexture(text_tar);
	}

	//Functionality
	[[nodiscard]] SDL_Renderer* GetRenderer() const { return renderer; }
	void BeginFrame() const {
		SDL_SetRenderTarget(renderer, nullptr);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
	}
	void EndFrame() const {
		SDL_RenderPresent(renderer);
	}

	void SetWinSize() {
		SDL_GetCurrentRenderOutputSize(renderer, &win_size.x, &win_size.y);
	}

	//Sprites
	void DrawSheet(const Sprite& sheet, const Vec2i& pos = { 0 }) const;
	void DrawSprite(const Sprite& spr) const;
	//Text
	void DrawTxt(Text& txt);

	void DrawGrid(const Vec2i& start = { 0, 0 }, const Vec2i& end = { 400 }, const uchar& tile_size = 16, const Color& grid_color = Color(1)) const;

	void DrawPath(const std::vector<Vec2i>& path, const Color& path_color = Color(1));

	//Drawing shapes
	void DrawLine(const Line& line, const Color& color = Color(1), const uchar edge_w = 1) const;
	void DrawCircle(const Circle& circle, const Color& fill_color = Color(1), const Color& stroke_color = Color(0, 0), const uchar edge_w = 1);
	void DrawTri(const Tri& tri, const Color& fill_color = Color(1), const Color& stroke_color = Color(0, 0), const uchar edge_w = 1);
	void DrawRect(const Rect& rect, const Color& fill_color = Color(1), const Color& stroke_color = Color(0, 0), const uchar edge_w = 1);

private:
	SDL_Renderer* renderer = nullptr;
	SDL_Surface* surface = nullptr;
	SDL_Texture* text_tar = nullptr;
	Camera* camera = nullptr;
	const Vec2i min_res = { 400 };
	Vec2i win_size;
};