#include <filesystem>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3/SDL_surface.h>
#include <string>
#include "TileMap.h"
#include "../../Entities/Door.h"

bool TileMap::Load(const string& json_file) {
	//Unload the old tilemap if the was one
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

	//Resize tile_data to be the size of the map
	tile_data.resize(map_size_t.x, vector<Tile>(map_size_t.y));

	//Load all of our needed textures
	string ts_name;
	SDL_Texture* tex = nullptr;
	for (const auto& ts : tilemap_data["tilesets"]) {
		ts_name = filesystem::path(ts["source"].get<string>()).stem().string();

		if (ts_name == "Unwalkable") continue;

		if (ts_name.substr(0, 4) == "Door") ts_name += "_2";

		tex = IMG_LoadTexture(renderer->GetRenderer(), ("../assets/Sprites/Tilesets/" + ts_name + ".png").c_str());
		if (!tex) {
			cerr << "Failed to load " << ts_name << ".png! Skipping this one.\n";
			continue;
		}

		SDL_SetTextureScaleMode(tex, SDL_SCALEMODE_NEAREST);

		tilesets[ts_name] = tex;
	}

	SDL_Texture* ts_tex;
	Vec2f tile_pos;
	uint global_tile_id, local_tile_id, firstgid;
	uint col = 0, row = 0;
	string layer_name;

	//Populate the map
	for (const auto& layer : tilemap_data["layers"]) {
		//Load the tiles
		if (layer["type"].get<string>() == "tilelayer") {
			for (col = 0; col < map_size_t.x; ++col) {
				for (row = 0; row < map_size_t.y; ++row) {
					//Get current tile ID number
					global_tile_id = layer["data"][col + row * map_size_t.x];
					//Skip empty tiles
					if (!global_tile_id) continue;

					//Tile info
					//Tile pos
					tile_pos = { col * TS, row * TS };
					tile_data[col][row].pos = tile_pos;
					//Walkability
					if (layer["name"].get<string>() == "Unwalkable")
						tile_data[col][row].walkable = false;

					//Get the name of the tileset
					for (const auto& tileset : tilemap_data["tilesets"]) {
						firstgid = tileset["firstgid"].get<int>();
						if (global_tile_id >= firstgid) {
							ts_name = filesystem::path(tileset["source"].get<string>()).stem().string();

							//Ensure a tileset was found
							if (ts_name.empty()) {
								cerr << "Warning: Tile ID " << global_tile_id << " has no matching tileset!\n";
								continue;
							}

							local_tile_id = global_tile_id - firstgid;
						}
						else break;
					}

					//Add the name of the tileset as a terrain type
					if (ts_name == "Stone" or ts_name == "Wood" or ts_name == "Grass" or ts_name == "Dirt" or ts_name == "Bridge")
						tile_data[col][row].terrain = Terrain::Normal;
					else if (ts_name == "Water" and tile_data[col][row].terrain != Terrain::Normal) {
						tile_data[col][row].terrain = Terrain::Water;
						tile_data[col][row].walkable = false;
					}

					//Get the texture
					if (tilesets.find(ts_name) != tilesets.end())
						ts_tex = tilesets[ts_name];
					else {
						cerr << "Warning: Tile ID " << global_tile_id << " has no matching tileset for " << ts_name << " Tile! Skipping\n";
						continue;
					}

					//Which layer are we submitting to?
					LayerName ln = layer["name"].get<string>() == "Structures" ? LayerName::Structures : LayerName::Ground;
					renderer->SubmitTile(ln, ts_tex, local_tile_id, tile_pos);
				}
			}
		}

		//Objects - doors, npcs, mobs, and party spawn points
		else if (layer["type"].get<string>() == "objectgroup") {
			//Doors
			if (layer["name"].get<string>() == "Doors") {
				float health = 0, armor = 0;
				bool locked = false, open = false, portal = false;
				AreaName to_area = AreaName::NONE;

				Sprite::Info door_info;
				door_info.default_layer = LayerName::Objects;
				door_info.origin = { 0, 1 };
				string door_type;
				Vec2f d_pos;
				for (const auto& d : layer["objects"]) {
					d_pos = {d.at("x"), d.at("y")};
					door_info.pos = Round(floor(d_pos.x / TS) * TS, floor(d_pos.y / TS) * TS);
					door_info.pos.x = door_info.pos.x + door_info.spr_size.x * door_info.origin.x;

					door_info.frame_size = { 24, 48};
					door_info.sheet = "Tilesets/" + d["type"].get<string>() + "_2";

					//Get the properties
					if (d.contains("properties")) {
						for (const auto& p : d.at("properties")) {
							if (p["name"].get<string>() == "Health") health = p["value"].get<float>();
							else if (p["name"].get<string>() == "Armor") armor = p["value"].get<float>();
							else if (p["name"].get<string>() == "Locked") locked = p["value"].get<bool>();
							else if (p["name"].get<string>() == "Open") open = p["value"].get<bool>();
							else if (p["name"].get<string>() == "Portal") portal = p["value"].get<bool>();
						}
					} else
						cerr << "TileMap::Load(): Door to " << d["name"].get<string>() << " has no properties!\n";

					//Where are we headed, if anywhere?
					if (d.contains("name"))
						to_area = StringToArea(d["name"].get<string>());

					doors.emplace_back(new Door(door_info, to_area, Size::Big, locked, open, portal, health, armor));
				}
			}

			//Mobs
			else if (layer["name"].get<string>() == "Mobs") {
				for (const auto& mob : layer["objects"])
					mobs.emplace_back(mob["name"].get<string>(), Round(floor(mob["x"].get<float>() / TS) * TS + TS * .5f, floor(mob["y"].get<float>() / TS) * TS + TS * .5f));
			}

			//NPCs
			else if (layer["name"].get<string>() == "NPCs") {
				for (const auto& npc : layer["objects"]) {
					if (StringToNPC(npc["name"].get<string>()) == NPCName::NONE) {
						cerr << "TileMap::Load(): Error! Trying to spawn an NPC that doesn't exist (" << npc["name"].get<string>() << ")!\n";
						continue;
					}
					npcs.emplace_back(StringToNPC(npc["name"].get<string>()), Round(floor(npc["x"].get<float>() / TS) * TS + TS * .5f, floor(npc["y"].get<float>() / TS) * TS + TS * .5f));
				}
			}

			//Spawn Points
			else if (layer["name"].get<string>() == "Spawn Points") {
				for (const auto& spawn : layer["objects"])
					spawn_points[spawn["name"].get<string>()] = Round(floor(spawn["x"].get<float>() / TS) * TS + TS * .5f, floor(spawn["y"].get<float>() / TS) * TS + TS * .5f);
			}
		}
	}

	return true;
}

void TileMap::Unload() {
	for (const auto& [ts_name, ts_tex] : tilesets)
		SDL_DestroyTexture(ts_tex);
	tilesets.clear();
	tile_data.clear();

	//Clear the batches in the Render Layers
	for (auto& l : renderer->layers)
		l.persistent_commands.clear();

	for (auto& d : doors) delete d;
	doors.clear();

	mobs.clear();
	npcs.clear();
	spawn_points.clear();
}

Vec2i TileMap::GetSpawnPoint(const string& spawn_point) const {
	if (spawn_points.find(spawn_point) != spawn_points.end()) return spawn_points.at(spawn_point);

	cerr << "TileMap::GetSpawnPoint(): Error! Could not determine a spawn point on this map! Returning {1, 1}\n";
	return { 1, 1 };
}