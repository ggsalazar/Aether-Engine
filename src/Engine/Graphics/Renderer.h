#pragma once
#include <algorithm>
#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <vector>

#include "../Enums.h"
#include "../Math/Geometry.h"

using std::vector, std::cout, std::endl;

class Camera;
class Sprite;
class Text;
class TileMap;

struct RRect {
	Rect rect;
	Color fill_color;
	Color stroke_color;
	uchar edge_w = 1;
};

struct Batch {
	LayerName layer = LayerName::NONE;

	SDL_Texture* texture = nullptr;
	vector<SDL_Vertex> vertices;
	vector<int> indices;
	Vec2f tex_size, norm_tex_size;

	bool tile = false;
};

struct DrawCommand {
	enum class Type {
		Batch,
		Rect,
		Text
	};
	Type type = Type::Batch;
	int layer_order = 0;
	Batch batch;
	std::pair<Text*, Rect> text;
	RRect rect;
};

struct RenderLayer {
	LayerName name = LayerName::NONE;
	int order = 0;
	float opacity = 1.0f;

	SDL_Texture* target = nullptr;

	//Tiles & static geometry
	vector<DrawCommand> persistent_commands;
	//Sprites, rects, texts
	vector<DrawCommand> transient_commands;
};

class Renderer {
public:
	vector<RenderLayer> layers;

	Renderer(SDL_Window* window);
	~Renderer() {
		if (renderer) SDL_DestroyRenderer(renderer);
		if (surface) SDL_DestroySurface(surface);
		if (text_tar) SDL_DestroyTexture(text_tar);
		for (auto& l : layers) {
			l.persistent_commands.clear();
			l.transient_commands.clear();
			SDL_DestroyTexture(l.target);
		}
	}
	void SetCamera(Camera* cam) { camera = cam; }
	void SetDrawScale(const uchar scale);

	//Functionality
	[[nodiscard]] SDL_Renderer* GetRenderer() const { return renderer; }
	void Render();
	void DrawLayer(RenderLayer& rl);
	void BeginFrame() const {
		SDL_SetRenderTarget(renderer, nullptr);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
	}
	void EndFrame() const {
		SDL_RenderPresent(renderer);
	}

	void SubmitSprite(const LayerName ln, const Sprite& spr);
	void SubmitTile(const LayerName ln, SDL_Texture* tex, const uint l_tile_id, const Vec2f tile_pos);
	void SubmitText(const LayerName ln, Text& text, const Rect& clip_rect);
	void SubmitRect(const LayerName ln, const Rect& rect, const Color& fill_color = Color(1), const Color& stroke_color = Color(0,0), const int layer_order = 0, const uchar edge_w = 1);

	void DrawGrid(const Vec2f& start = { 0, 0 }, const Vec2f& end = { 400 }, const float& tile_size = 16, const Color& grid_color = Color(1)) const;

	void DrawPath(const std::vector<Vec2i>& path, const Color& path_color = Color(1));

private:
	SDL_Renderer* renderer = nullptr;
	SDL_Surface* surface = nullptr;
	SDL_Texture* text_tar = nullptr;
	Camera* camera = nullptr;
	const Vec2i min_res = { 640, 360 };

	void DrawSprite(const Sprite& spr, const RenderLayer& layer) const;
	void DrawTilemap(Batch& batch) const;
	void DrawTxt(Text& txt, const Rect& clip_rect);

	void DrawLine(const Line& line, const Color& color = Color(1), const uchar edge_w = 1) const;
	void DrawRect(const RRect& rect);
	void DrawCircle(const Circle& circle, const Color& fill_color = Color(1), const Color& stroke_color = Color(0, 0), const uchar edge_w = 1);
	void DrawTri(const Tri& tri, const Color& fill_color = Color(1), const Color& stroke_color = Color(0, 0), const uchar edge_w = 1);
};