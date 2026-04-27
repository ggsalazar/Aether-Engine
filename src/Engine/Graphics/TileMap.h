#pragma once
#include <fstream>
#include <vector>
#include <nlohmann/json.hpp>
#include "Renderer.h"
#include "Sprite.h" //Geometry (Vec2 (iostream))
#include "../Enums.h"

using json = nlohmann::json;
using std::vector, std::cerr, std::cout, std::unordered_map, std::string, std::pair;

//Tile Size
constexpr float TS = 8;
constexpr float METER = 16;

class Door;

struct Tile {
    Vec2u pos;
    bool walkable = true;
    Terrain terrain = Terrain::NONE;
};

class TileMap {
    friend class Renderer;
    friend class Pathfinding;
public:
    TileMap() = default;

    inline static void SetRenderer(Renderer* r) { renderer = r; }

    //Tiling
    bool Load(const string& json_file);
    void Unload();

    inline bool Loaded() const { return map_size_t.x != 0 and map_size_t.y != 0; }

    [[nodiscard]] inline Vec2u GetMapSizeTiles() const { return map_size_t; }
    [[nodiscard]] inline Vec2u GetMapSizePixels() const { return map_size_p; }

    [[nodiscard]] inline Tile GetTileData(const Vec2i tile_pos) const { return tile_data[floor(tile_pos.x)][floor(tile_pos.y)]; }

    inline bool HasDoors() const { return doors.size(); }
    inline vector<Door*>* GetDoors() { return &doors; }
    inline bool HasMobs() const { return mobs.size(); }
    inline vector<pair<string, Vec2i>>* GetMobs() { return &mobs; }
    inline bool HasNPCs() const { return npcs.size(); }
    inline vector<pair<NPCName, Vec2i>>* GetNPCs() { return &npcs; }
    [[nodiscard]] Vec2i GetSpawnPoint(const string& spawn_point) const;

private:
    inline static Renderer* renderer = nullptr;

    unordered_map<string, Batch> tile_batches;

    unordered_map<string, SDL_Texture*> tilesets;

    json tilemap_data;
    vector<vector<Tile>> tile_data;

    vector<Door*> doors;
    vector<pair<string, Vec2i>> mobs;
    vector<pair<NPCName, Vec2i>> npcs;
    unordered_map<string, Vec2i> spawn_points;

    Vec2u map_size_t = { 0, 0 };
    Vec2u map_size_p = { 0, 0 };
};