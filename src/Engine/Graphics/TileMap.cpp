#include <string>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3/SDL_surface.h>
#include "TileMap.h"
#include "../../Entities/Portal.h"

bool TileMap::Load(const string& json_file) {
	//Unload the old tilemap, if there was one
	Unload();

	//Load json file
	std::ifstream file("../assets/Maps/" + json_file + ".json");
	if (!file.is_open()) {
		cerr << "Failed to open " << json_file << " map!\n";
		return false;
	}

	file >> tilemap_data;

	//Map size in tiles/pixels
	map_size_t = { tilemap_data["width"], tilemap_data["height"] };
	map_size_p = map_size_t * TS;

	//Resize tile_data
	tile_data.resize(map_size_t.x, vector<Tile>(map_size_t.y));

	//Load all of our needed textures
	tilesets["Default"] = IMG_LoadTexture(renderer, "../assets/Sprites/Environments/Tilesets/Default.png");
	SDL_SetTextureScaleMode(tilesets["Default"], SDL_SCALEMODE_NEAREST);
	string ts_name;
	for (const auto& ts : tilemap_data["tilesets"]) {
		ts_name = ts["source"];
		ts_name = ts_name.erase(ts_name.length() - 4); //Remove filename extension
		tilesets[ts_name] = IMG_LoadTexture(renderer, ("../assets/Sprites/Environments/Tilesets/" + ts_name + ".png").c_str());
		SDL_SetTextureScaleMode(tilesets[ts_name], SDL_SCALEMODE_NEAREST);
	}

	SDL_Texture* ts_tex;
	Vec2f tile_pos, tex_size;
	Vec2u tile_uv, vert_uv;
	SDL_Vertex vert[4];
	int t_p_r, base;
	uint global_tile_id, local_tile_id, firstgid;
	uint col = 0, row = 0;

	//Populate tile_data / the map itself
	for (const auto& layer : tilemap_data["layers"]) {
		//Load the tiles
		if (layer["type"] == "tilelayer") {
			for (col = 0; col < map_size_t.x; ++col) {
				for (row = 0; row < map_size_t.y; ++row) {
					//Get current tile ID number
					global_tile_id = layer["data"][col + row * map_size_t.x];

					//Skip empty tiles
					if (!global_tile_id) continue;

					//Get the name of the tileset
					for (const auto& tileset : tilemap_data["tilesets"]) {
						firstgid = tileset["firstgid"];
						if (global_tile_id >= firstgid) {
							ts_name = tileset["source"];
							//Ensure a tileset was found
							if (ts_name.empty()) {
								cerr << "Warning: Tile ID " << global_tile_id << " has no matching tileset!\n";
								continue;
							}

							ts_name.erase(ts_name.length() - 4); //Remove ".tsx"
							local_tile_id = global_tile_id - firstgid;
						}
						else break;
					}

					//Add the data of the current tile to the tile_data 2D vector
					tile_data[col][row].pos = { col * TS, row * TS };

					if (layer["name"] == "Unwalkable")
						tile_data[col][row].walkable = false;

					else {
						//Add the name of the tileset as a terrain type
						if (ts_name == "Stone" or ts_name == "Wood" or ts_name == "Grass" or ts_name == "Dirt")
							tile_data[col][row].terrain = Terrain::Normal;
						else if (ts_name == "Water") {
							tile_data[col][row].terrain = Terrain::Water;
							tile_data[col][row].walkable = false;
						}

						//Populate vertex buffer
						tile_pos = { (float)(col * TS), (float)(row * TS) };

						//Set vertex positions
						vert[0].position = { tile_pos.x, tile_pos.y };
						vert[1].position = { tile_pos.x + TS, tile_pos.y };
						vert[2].position = { tile_pos.x + TS, tile_pos.y + TS };
						vert[3].position = { tile_pos.x, tile_pos.y + TS };

						//Get the texture
						if (tilesets.find(ts_name) != tilesets.end())
							ts_tex = tilesets[ts_name];
						else {
							cerr << "Warning: Tile ID " << global_tile_id << " has no matching tileset for " << ts_name << " Tile! Assigning Default\n";
							ts_tex = tilesets["Default"];
						}
						SDL_GetTextureSize(ts_tex, &tex_size.x, &tex_size.y);
						t_p_r = tex_size.x / TS;
						tile_uv = { local_tile_id % t_p_r, local_tile_id / t_p_r };
						vert_uv = tile_uv * TS;

						vert[0].tex_coord = { vert_uv.x / tex_size.x, vert_uv.y / tex_size.y };
						vert[1].tex_coord = { (vert_uv.x + TS) / tex_size.x, vert_uv.y / tex_size.y };
						vert[2].tex_coord = { (vert_uv.x + TS) / tex_size.x, (vert_uv.y + TS) / tex_size.y };
						vert[3].tex_coord = { vert_uv.x / tex_size.x, (vert_uv.y + TS) / tex_size.y };

						for (int i = 0; i < 4; ++i)
							vert[i].color = { 1.f, 1.f, 1.f, 1.f };

						auto& verts = verts_by_tileset[ts_name];
						auto& inds = indices_by_tileset[ts_name];
						base = verts.size();

						verts.insert(verts.end(), std::begin(vert), std::end(vert));
						inds.insert(inds.end(), { base, base + 1, base + 2, base, base + 2, base + 3 });
					}
				}
			}
		}

		/*
		//Transitions/Doors, decorations, etc
		else if (layer["type"] == "objectgroup") {
			//Doors
			if (layer["name"] == "Portals") {
				Sprite::Info portal_info = {};
				//Bear in mind that the origin point of objects in tiled is (0, 1) (bottom left)
				portal_info.origin = {.5f, 1};


				Vec2f p_pos;
				string to_area;
				for (const auto& p : layer["objects"]) {
					p_pos = {p["x"], p["y"]};
					portal_info.pos = Vec2f{floor(p_pos.x / TS) * TS, floor(p_pos.y / TS) * TS};
					portal_info.spr_size = { p["width"], p["height"] };
					portal_info.pos.x = portal_info.pos.x + portal_info.spr_size.x * portal_info.origin.x;
					portal_info.dfc = -portal_info.pos.y - 128;

					portal_info.sheet = "Environments/Portals/Door";
					to_area = p["name"];

					portals.insert({StringToArea(to_area), new Portal(portal_info, StringToArea(to_area))});
				}
			}
		}

		//Transitions, decorations, etc
		else if (layer["type"] == "objectgroup") {

			//Place decorations
			if (layer["name"] == "Decorations") {

				Sprite::Info decor_i = {};
				//Tiled assumes bottom-left origin which is normally great (Cartesian coordinates!), but is contrary to how I (and SDL) have implemented sprites
				decor_i.origin = { 0.f, 1.f };

				for (const auto& object : layer["objects"]) {
					//Don't need to set size since most (if not all) structures should be 1 frame
					//Will actually need to set size while using placeholders since I'm scaling the sprites up in Tiled
					decor_i.spr_size = { object["width"], object["height"] };
					decor_i.pos = { object["x"], object["y"] };
					decor_i.sheet = "Environments/Structures/" + object["name"].get<string>();
					decor_i.dfc = -decor_i.pos.y;

					decorations.push_back(Sprite(decor_i));
				}
			}

			//Get the doors
			else if (layer["name"] == "Internal Area Transitions") {
				float health = 0, armor = 0;
				bool locked = false;

				Sprite::Info door_info = {};
				door_info.origin = { .5f, 1 };
				string door_type = "";

				for (const auto& d : layer["objects"]) {
					door_info.pos = Round(floor(d["x"] / TS) * TS, floor(d["y"] / TS) * TS);
					door_info.spr_size = { d["width"], d["height"] };
					door_info.pos = Round(door_info.pos.x + door_info.spr_size.x * .5f, door_info.pos.y + door_info.spr_size.y);
					door_info.dfc = -door_info.pos.y - 128;

					door_type = d["type"];
					if (door_type == "DoorF") door_info.frame_size = { 24, 22 };
					else if (door_type == "DoorR" or door_type == "DoorL") door_info.frame_size = { 8, 12 };
					door_info.sheet = "Environments/Structures/" + door_type;

					//Get the properties
					for (const auto& p : d["properties"]) {
						if (p["name"] == "Health") health = p["value"];
						if (p["name"] == "Armor") armor = p["value"];
						if (p["name"] == "Locked") locked = p["value"];
					}

					transitions.push_back(new Door(door_info, StringToArea(d["name"]), health, armor, locked));
				}
			}

			//Get the spawn points
			else if (layer["name"] == "Spawn Points") {
				for (const auto& spawn : layer["objects"]) {
					//Get the name
					string name = spawn["name"];

					//Get the position and place it on the nearest grid point
					//Add it to the map
					spawn_points[name] = Round(floor(spawn["x"] / TS) * TS + TS * .5f, floor(spawn["y"] / TS) * TS + TS * .5f);
				}
			}
		}
		*/
	}

	return true;
}

void TileMap::Unload() {
	for (const auto& [ts_name, ts_tex] : tilesets)
		SDL_DestroyTexture(ts_tex);

	verts_by_tileset.clear();
	indices_by_tileset.clear();
	tilesets.clear();
	tile_data.clear();

	//for (auto& d : decorations) delete d;
	//decorations.clear();
	//for (auto& [_, p] : portals) delete p;
	//portals.clear();
}