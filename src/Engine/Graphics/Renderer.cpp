#include <sstream>
#include "Renderer.h"
#include "Camera.h"
#include "Sprite.h"
#include "Text.h" //Font
#include "TileMap.h"
#include "../Collision.h"

Renderer::Renderer(SDL_Window* window) {

	SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");
	renderer = SDL_CreateRenderer(window, nullptr);

	//Set the renderer to *logically* render things at the minimum resolution (400x400), then scale it up to the window at rendering time
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderLogicalPresentation(renderer, min_res.x, min_res.y, SDL_LOGICAL_PRESENTATION_INTEGER_SCALE);

	//Initialize the Render Layers
	for (int i = 0; i < scst(LayerName::COUNT); ++i) {
		auto& l = layers.emplace_back(static_cast<LayerName>(i), i);
		l.target = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, min_res.x, min_res.y);
		SDL_SetTextureBlendMode(l.target, SDL_BLENDMODE_BLEND);
		SDL_SetTextureScaleMode(l.target, SDL_SCALEMODE_NEAREST);
	}
}

void Renderer::SetDrawScale(const uchar scale) {
	SDL_SetRenderScale(renderer, scale, scale);
}

void Renderer::Render() {
	//Render into each layer
	for (auto& l : layers) {
		if (l.opacity == 0.f) continue;

		SDL_SetRenderTarget(renderer, l.target);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		SDL_RenderClear(renderer);

		//Sort the commands vectors by layer order
		std::sort(l.persistent_commands.begin(), l.persistent_commands.end(), [](const DrawCommand& a, const DrawCommand& b) { return a.layer_order < b.layer_order; });
		std::sort(l.transient_commands.begin(), l.transient_commands.end(), [](const DrawCommand& a, const DrawCommand& b) { return a.layer_order < b.layer_order; });

		//Draw each thing in the layer
		DrawLayer(l);
	}

	//Composite the layers
	SDL_SetRenderTarget(renderer, nullptr);
	for (auto& l : layers) {
		if (l.opacity == 0.f) continue;

		SDL_SetTextureAlphaModFloat(l.target, l.opacity);
		SDL_RenderTexture(renderer, l.target, nullptr, nullptr);
	}
}

void Renderer::DrawLayer(RenderLayer& rl) {
	for (auto& c : rl.persistent_commands) {
		switch (c.type) {
			case DrawCommand::Type::Batch: {
				auto& b = c.batch;
				if (b.vertices.size()) {
					if (b.tile)
						DrawTilemap(b);
					else {
						SDL_RenderGeometry(
							renderer, b.texture,
					   b.vertices.data(),
					   b.vertices.size(),
					   b.indices.data(),
					   b.indices.size());

						b.vertices.clear();
						b.indices.clear();
					}
				}
				break;
			}

			case DrawCommand::Type::Rect:
				DrawRect(c.rect);
				break;

			case DrawCommand::Type::Text:
				DrawTxt(*c.text.first, c.text.second);
				break;
		}
	}

	for (auto& c : rl.transient_commands) {
		switch (c.type) {
			case DrawCommand::Type::Batch: {
				auto& b = c.batch;
				if (b.vertices.size()) {
					SDL_RenderGeometry(
						renderer, b.texture,
				   b.vertices.data(),
				   b.vertices.size(),
				   b.indices.data(),
				   b.indices.size());

					b.vertices.clear();
					b.indices.clear();
				}
				break;
			}

			case DrawCommand::Type::Rect:
				DrawRect(c.rect);
				break;

			case DrawCommand::Type::Text:
				DrawTxt(*c.text.first, c.text.second);
				break;
		}
	}
	rl.transient_commands.clear();
}

void Renderer::SubmitSprite(const LayerName ln, const Sprite& spr) {
	Batch* batch = nullptr;
	for (auto& c : layers[scst(ln)].transient_commands) {
		if (c.type == DrawCommand::Type::Batch and c.batch.texture == spr.texture and c.layer_order == spr.GetLayerOrder()) {
			batch = &c.batch;
			break;
		}
	}
	if (!batch) {
		auto& b = layers[scst(ln)].transient_commands.emplace_back(DrawCommand::Type::Batch, spr.GetLayerOrder());
		batch = &b.batch;
		batch->layer = ln;
		batch->texture = spr.texture;
		SDL_GetTextureSize(batch->texture, &batch->tex_size.x, &batch->tex_size.y);
		batch->norm_tex_size = { 1.f / batch->tex_size.x, 1.f / batch->tex_size.y };
	}

	const Sprite::Info* si = &spr.info;

	//Only draw sprites if they will be seen by the camera
	Vec2i sprite_pos = Round(si->pos.x - (si->spr_size.x * si->origin.x),
								si->pos.y - (si->spr_size.y * si->origin.y));

	if (Collision::AABB(camera->viewport, Rect(sprite_pos, si->spr_size))) {
		size_t base = batch->vertices.size();
		SDL_Vertex vert[4];

		//Vertex position in world space
		const SDL_FRect dest = { (float)sprite_pos.x - camera->viewport.x,
								(float)sprite_pos.y - camera->viewport.y,
								(float)si->spr_size.x,
								(float)si->spr_size.y };
		vert[0].position = {dest.x, dest.y};
		vert[1].position = {dest.x + dest.w, dest.y};
		vert[2].position = {dest.x + dest.w, dest.y + dest.h};
		vert[3].position = {dest.x, dest.y + dest.h};

		//Vertex texture coordinates (Normalized 0 - 1)
		SDL_FRect src = { (float)si->frame * si->frame_size.x * batch->norm_tex_size.x, 0,
			(float)si->frame_size.x * batch->norm_tex_size.x,
			(float)si->frame_size.y * batch->norm_tex_size.y };
		vert[0].tex_coord = {src.x, src.y};
		vert[1].tex_coord = {src.x + src.w, src.y};
		vert[2].tex_coord = {src.x + src.w, src.y + src.h};
		vert[3].tex_coord = {src.x, src.y + src.h};

		//Vertex color
		for (int i = 0; i < 4; ++i)
			vert[i].color = { si->tint.r, si->tint.g, si->tint.b, si->tint.a };

		batch->vertices.insert(batch->vertices.end(), std::begin(vert), std::end(vert));

		//Indices
		batch->indices.push_back(base);
		batch->indices.push_back(base + 1);
		batch->indices.push_back(base + 2);
		batch->indices.push_back(base + 2);
		batch->indices.push_back(base + 3);
		batch->indices.push_back(base);
	}
}

void Renderer::SubmitTile(const LayerName ln, SDL_Texture* tex, const uint l_tile_id, const Vec2f tile_pos) {
	Batch* batch = nullptr;
	for (auto& c : layers[scst(ln)].persistent_commands) {
		if (c.type == DrawCommand::Type::Batch and c.batch.texture == tex) {
			batch = &c.batch;
			break;
		}
	}
	if (!batch) {
		//batch layer_order stays 0
		auto& b = layers[scst(ln)].persistent_commands.emplace_back(DrawCommand::Type::Batch);
		batch = &b.batch;
		batch->layer = ln;
		batch->texture = tex;
		batch->tile = true;
		SDL_GetTextureSize(batch->texture, &batch->tex_size.x, &batch->tex_size.y);
		batch->norm_tex_size = { 1.f / batch->tex_size.x, 1.f / batch->tex_size.y };
	}

	//tiles_per_row
	int t_p_r = batch->tex_size.x / TS;
	Vec2f tile_uv = Vec2{l_tile_id % t_p_r, l_tile_id / t_p_r};
	Vec2f vert_uv = tile_uv * TS;

	//Set vertex positions & tex_coords
	SDL_Vertex vert[4];
	vert[0].position = { tile_pos.x, tile_pos.y };
	vert[1].position = { tile_pos.x + TS, tile_pos.y };
	vert[2].position = { tile_pos.x + TS, tile_pos.y + TS };
	vert[3].position = { tile_pos.x, tile_pos.y + TS };

	vert[0].tex_coord = { vert_uv.x * batch->norm_tex_size.x, vert_uv.y * batch->norm_tex_size.y };
	vert[1].tex_coord = { (vert_uv.x + TS) * batch->norm_tex_size.x, vert_uv.y * batch->norm_tex_size.y };
	vert[2].tex_coord = { (vert_uv.x + TS) * batch->norm_tex_size.x, (vert_uv.y + TS) * batch->norm_tex_size.y };
	vert[3].tex_coord = { vert_uv.x * batch->norm_tex_size.x, (vert_uv.y + TS) * batch->norm_tex_size.y };

	for (int i = 0; i < 4; ++i)
		vert[i].color = { 1.f, 1.f, 1.f, 1.f };

	int base = batch->vertices.size();
	batch->vertices.insert(batch->vertices.end(), std::begin(vert), std::end(vert));
	batch->indices.insert(batch->indices.end(), { base, base + 1, base + 2, base, base + 2, base + 3 });
}

void Renderer::SubmitText(const LayerName ln, Text& text, const Rect& clip_rect) {
	auto& t = layers[scst(ln)].transient_commands.emplace_back(DrawCommand::Type::Text, text.GetLayerOrder());
	t.text = std::make_pair(&text, clip_rect);
}

void Renderer::SubmitRect(const LayerName ln, const Rect& rect, const Color& fill_color, const Color& stroke_color, const int layer_order, const uchar edge_w) {
	auto& r = layers[scst(ln)].transient_commands.emplace_back(DrawCommand::Type::Rect, layer_order);
	r.rect = RRect(rect, fill_color, stroke_color, edge_w);
}

void Renderer::DrawSprite(const Sprite& spr, const RenderLayer& layer) const {
	const Sprite::Info* si = &spr.info;

	//Only draw sprites if they will be seen by the camera
	Vec2i sprite_pos = Round(si->pos.x - (si->spr_size.x * si->origin.x),
								si->pos.y - (si->spr_size.y * si->origin.y));
	if (Collision::AABB(camera->viewport, Rect(sprite_pos, si->spr_size))) {
		const SDL_FRect src = { (float)si->frame * si->frame_size.x, 0,
								(float)si->frame_size.x,	(float)si->frame_size.y };


		const SDL_FRect dest = { (float)sprite_pos.x - camera->viewport.x,
								(float)sprite_pos.y - camera->viewport.y,
								(float)si->spr_size.x,
								(float)si->spr_size.y };

		//Set the center of rotation
		//SDL uses degrees, as do my sprites
		const SDL_FPoint center = {dest.w * .5f, dest.h * .5f};
		//Flip is handled by scale; -1 values for x/y will flip it appropriately

		//Set the tint
		SDL_SetTextureColorMod(spr.texture, si->tint.r * 255, si->tint.g * 255, si->tint.b * 255);
		SDL_SetTextureAlphaMod(spr.texture, si->tint.a * layer.opacity * 255);

		SDL_RenderTextureRotated(renderer, spr.texture, &src, &dest, si->rot, &center, SDL_FLIP_NONE);
	}
}

void Renderer::DrawTilemap(Batch& batch) const {

	size_t base_index;
	vector<SDL_Vertex> transformed;
	vector<int> culled_inds;
	auto& verts = batch.vertices;
	Vec2f v_pos;

	for (size_t i=0; i < verts.size(); i+=4) {
		v_pos = { verts[i].position.x, verts[i].position.y};
		if (!Collision::AABB(camera->viewport, Rect(v_pos, TS)))
			continue;

		base_index = transformed.size();
		for (uchar v=0; v<4; ++v) {
			transformed.push_back(SDL_Vertex{
				verts[i+v].position.x - camera->viewport.x, verts[i+v].position.y - camera->viewport.y,
				verts[i+v].color,
				verts[i+v].tex_coord});
		}

		//Add indices for this tile
		culled_inds.push_back(base_index);
		culled_inds.push_back(base_index+1);
		culled_inds.push_back(base_index+2);
		culled_inds.push_back(base_index+2);
		culled_inds.push_back(base_index+3);
		culled_inds.push_back(base_index);
	}

	if (!transformed.empty())
		SDL_RenderGeometry(renderer, batch.texture,
			transformed.data(),transformed.size(),
			culled_inds.data(),culled_inds.size());
}

void Renderer::DrawTxt(Text& txt, const Rect& clip_rect) {
	Text::Info* ti = &txt.info;

	SDL_Color c = {
		static_cast<Uint8>(ti->color.r * 255),
		static_cast<Uint8>(ti->color.g * 255),
		static_cast<Uint8>(ti->color.b * 255),
		static_cast<Uint8>(ti->color.a * 255)
	};
	if (ti->str.empty() or !c.a)
		return;

	if (!txt.font.GetFont()) {
		cout << "Font is null\n";
		return;
	}
	//Text wrapping has to be done manually to account for alignment
	std::vector<string> lines;
	std::istringstream full_stream(ti->str);
	string full_text, curr_line, test, word;
	int w = 0;

	//Wrap the text
	while (std::getline(full_stream, full_text, '\n')) {
		std::istringstream line_stream(full_text);
		word = "";
		curr_line = "";
		while (line_stream >> word) {
			test = curr_line.empty() ? word : curr_line + " " + word;
			TTF_GetStringSize(txt.font.GetFont(), test.c_str(), test.length(), &w, nullptr);
			if (w > txt.GetMaxW() and !curr_line.empty()) {
				lines.push_back(curr_line);
				curr_line = word;
			}
			else
				curr_line = test;
		}
		if (!curr_line.empty()) lines.push_back(curr_line);
	}

	//Draw the text
	Vec2i txt_pos, line_size = {0, TTF_GetFontLineSkip(txt.font.GetFont()) - ti->line_height_offset};
	string line;
	for (int i = 0; i < lines.size(); ++i) {
		line = lines[i];

		//Surface and texture
		if (surface) SDL_DestroySurface(surface);
		surface = TTF_RenderText_Blended(txt.font.GetFont(), line.c_str(), line.length(), c);
		if (!surface) {
			std::cout << "Failed to create text surface!\n";
			return;
		}
		if (text_tar) SDL_DestroyTexture(text_tar);
		text_tar = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_SetTextureScaleMode(text_tar, SDL_SCALEMODE_NEAREST);
		if (!text_tar) {
			std::cout << "Failed to create text texture!\n";
			return;
		}

		//Get the size of the current line
		TTF_GetStringSize(txt.font.GetFont(), line.c_str(), line.length(), &line_size.x, nullptr);

		txt_pos = ti->pos;
		txt_pos = Round(txt_pos.x - round(line_size.x * ti->origin.x), txt_pos.y - round(line_size.y * ti->origin.y * lines.size()));
		//Only draw lines that can be seen by the camera
		if (Collision::AABB(camera->viewport, Rect(txt_pos, txt.GetStrSize()))) {
			//Enable ClipRect
			SDL_Rect cr = {static_cast<int>(clip_rect.x  - camera->viewport.x),
				static_cast<int>(clip_rect.y - camera->viewport.y),
				static_cast<int>(clip_rect.w),
				static_cast<int>(clip_rect.h)};
			if (cr.w == 0) cr = {0, 0, (int)camera->viewport.w, (int)camera->viewport.h};
			SDL_SetRenderClipRect(renderer, &cr);
			SDL_FRect src_rect = { 0, 0, (float)surface->w, (float)surface->h };
			SDL_FRect dest_rect = {
				(txt_pos.x - camera->viewport.x),
				(txt_pos.y + line_size.y*i - camera->viewport.y),
				(float)surface->w, (float)surface->h
			};
			SDL_RenderTexture(renderer, text_tar, &src_rect, &dest_rect);
			//Disable ClipRect
			SDL_SetRenderClipRect(renderer, nullptr);
		}
	}
}

void Renderer::DrawGrid(const Vec2f& start, const Vec2f& end, const float& tile_size, const Color& grid_color) const {
	//Vertical Lines
	for (float i = start.x; i <= end.x; i += tile_size)
		DrawLine(Line{ {i, start.y}, {i, end.y} }, grid_color);
	//Horizontal lines
	for (float i = start.y; i <= end.y; i += tile_size)
		DrawLine(Line{ {start.x, i}, {end.x, i} }, grid_color);
}

void Renderer::DrawPath(const vector<Vec2i>& path, const Color& path_color) {
	Rect point_box = { {0}, 2 };
	for (const auto& point : path) {
		point_box.x = point.x - 1; point_box.y = point.y - 1;
		DrawRect(RRect(point_box, path_color));
	}
}

void Renderer::DrawLine(const Line& line, const Color& color, const uchar edge_w) const {
	//SHOULD only draw if colliding with the camera but that requires implementing Collision::LineRect
	SDL_SetRenderDrawColor(renderer, color.r * 255, color.g * 255, color.b * 255, color.a * 255);

	SDL_RenderLine(renderer, line.pos1.x - camera->viewport.x, line.pos1.y - camera->viewport.y, line.pos2.x - camera->viewport.x, line.pos2.y - camera->viewport.y);
}

void Renderer::DrawCircle(const Circle& circle, const Color& fill_color, const Color& stroke_color, const uchar edge_w) {
	//Only draw if colliding with the camera
	if (Collision::RectCircle(camera->viewport, circle)) {
		const Vec2f circle_pos = { (circle.pos.x - camera->viewport.x), (circle.pos.y - camera->viewport.y) };
		float inner_r = circle.r - edge_w;

		//Outline
		if (stroke_color.a) {
			SDL_SetRenderDrawColor(renderer, stroke_color.r * 255, stroke_color.g * 255, stroke_color.b * 255, stroke_color.a * 255);
			for (int w = -circle.r; w <= circle.r; ++w) {
				for (int h = -circle.r; h <= circle.r; ++h) {
					float dist_sq = w * w + h * h;
					if (dist_sq <= circle.r * circle.r and dist_sq >= inner_r * inner_r)
						SDL_RenderPoint(renderer, circle_pos.x + w, circle_pos.y + h);
				}
			}
		}

		//Fill
		if (fill_color.a) {
			SDL_SetRenderDrawColor(renderer, fill_color.r * 255, fill_color.g * 255, fill_color.b * 255, fill_color.a * 255);
			for (int w = -inner_r; w <= inner_r; ++w) {
				for (int h = -inner_r; h <= inner_r; ++h) {
					if (w * w + h * h <= inner_r * inner_r)
						SDL_RenderPoint(renderer, circle_pos.x + w, circle_pos.y + h);
				}
			}
		}
	}
}

void Renderer::DrawTri(const Tri& tri, const Color& fill_color, const Color& stroke_color, const uchar edge_w) {
	//SHOULD only draw if colliding with the camera but that requires implementing triangle collisions so fuck that
	const Vec2f tri_pos1 = { (float)(tri.pos1.x - camera->viewport.x), (float)(tri.pos1.y - camera->viewport.y) };
	const Vec2f tri_pos2 = { (float)(tri.pos2.x - camera->viewport.x), (float)(tri.pos2.y - camera->viewport.y) };
	const Vec2f tri_pos3 = { (float)(tri.pos3.x - camera->viewport.x), (float)(tri.pos3.y - camera->viewport.y) };

	if (fill_color.a) {
		SDL_FColor f_color = { fill_color.r * 255, fill_color.g * 255, fill_color.b * 255, fill_color.a * 255 };

		//Draw the triangle
		SDL_Vertex verts[3];

		verts[0].position = { tri_pos1.x, tri_pos1.y };
		verts[0].color = f_color;

		verts[1].position = { tri_pos2.x, tri_pos2.y };
		verts[1].color = f_color;

		verts[2].position = { tri_pos3.x, tri_pos3.y };
		verts[2].color = f_color;

		SDL_RenderGeometry(renderer, nullptr, verts, 3, nullptr, 0);
	}

	//Draw the edges
	if (stroke_color.a) {
		SDL_SetRenderDrawColor(renderer, stroke_color.r * 255, stroke_color.g * 255, stroke_color.b * 255, stroke_color.a * 255);
		SDL_RenderLine(renderer, tri_pos1.x, tri_pos1.y, tri_pos2.x, tri_pos2.y);
		SDL_RenderLine(renderer, tri_pos2.x, tri_pos2.y, tri_pos3.x, tri_pos3.y);
		SDL_RenderLine(renderer, tri_pos3.x, tri_pos3.y, tri_pos1.x, tri_pos1.y);
	}
}

void Renderer::DrawRect(const RRect& rect) {
	//Only draw if colliding with the camera
	if (Collision::AABB(rect.rect, camera->viewport)) {
		Vec2f rect_pos = { (float)(rect.rect.x - camera->viewport.x), (float)(rect.rect.y - camera->viewport.y) };
		float w = rect.rect.w;
		float h = rect.rect.h;
		//Normalize w/h
		if (w < 0) {
			rect_pos.x += w;
			w = -w;
		}
		if (h < 0) {
			rect_pos.y += h;
			h = -h;
		}
		//Draw the fill
		if (rect.fill_color.a) {
			SDL_SetRenderDrawColor(renderer, rect.fill_color.r * 255, rect.fill_color.g * 255, rect.fill_color.b * 255, rect.fill_color.a * 255);
			SDL_FRect sdl_rect = { rect_pos.x, rect_pos.y, w, h };
			SDL_RenderFillRect(renderer, &sdl_rect);
		}

		//Draw the edges
		if (rect.stroke_color.a) {
			SDL_SetRenderDrawColor(renderer, rect.stroke_color.r * 255, rect.stroke_color.g * 255, rect.stroke_color.b * 255, rect.stroke_color.a * 255);
			//Top
			SDL_FRect top = { rect_pos.x, rect_pos.y, w, (float)rect.edge_w };
			SDL_RenderFillRect(renderer, &top);
			//Bottom
			SDL_FRect bot = { rect_pos.x, (rect_pos.y + h) - rect.edge_w, w, (float)rect.edge_w };
			SDL_RenderFillRect(renderer, &bot);
			//Left
			SDL_FRect left = { rect_pos.x, rect_pos.y + rect.edge_w, (float)rect.edge_w, h - (rect.edge_w * 2) };
			SDL_RenderFillRect(renderer, &left);
			//Right
			SDL_FRect right = { rect_pos.x + w - rect.edge_w, rect_pos.y + rect.edge_w, (float)rect.edge_w, h - rect.edge_w * 2 };
			SDL_RenderFillRect(renderer, &right);
		}
	}
}
