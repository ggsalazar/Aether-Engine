#pragma once
#include <fstream>
#include <vector>
#include <SDL3/SDL_render.h>
#include <nlohmann/json.hpp>
#include "../Enums.h"
#include "../Math/Geometry.h"

using json = nlohmann::json;
using std::vector, std::cerr, std::cout, std::unordered_map, std::string;

//Tile Size (for now)
const int TS = 16;

class Portal;

struct Tile {
    Vec2u pos;
    bool walkable = true;
    Terrain terrain = Terrain::NONE;
};

class TileMap {
    friend class Renderer;
public:
    TileMap() = default;

    void SetSDLRenderer(SDL_Renderer* r) { renderer = r; }

    //Tiling
    bool Load(const string& json_file);
    void Unload();

    inline bool Loaded() const { return map_size_t.x != 0 and map_size_t.y != 0; }

    [[nodiscard]] inline Vec2u GetMapSizeTiles() const { return map_size_t; }
    [[nodiscard]] inline Vec2u GetMapSizePixels() const { return map_size_p; }

    [[nodiscard]] inline Tile GetTileData(Vec2i tile_pos) const { return tile_data[floor(tile_pos.x)][floor(tile_pos.y)]; }

private:
    //SDL stuff
    SDL_Renderer* renderer = nullptr;
    unordered_map<string, vector<SDL_Vertex>> verts_by_tileset;
    unordered_map<string, vector<int>> indices_by_tileset;
    unordered_map<string, SDL_Texture*> tilesets;

    //Map stuff
    json tilemap_data;
    vector<vector<Tile>> tile_data;
    Vec2u map_size_t = { 0, 0 };
    Vec2u map_size_p = { 0, 0 };
};