#pragma once
#include <iostream>
#include <string>
#include <unordered_map>
#include <SDL3_image/SDL_image.h>

using std::string, std::unordered_map;

class TextureManager {
public:
    TextureManager() = default;
    ~TextureManager() {
        for (auto& [_, t] : textures)
            SDL_DestroyTexture(t);
        textures.clear();
    }

    static inline void SetSDLRenderer(SDL_Renderer* r) { sdl_renderer = r; }

    SDL_Texture* LoadTexture(string path) {
        path = "../assets/Sprites/" + path;

        if (textures.find(path) != textures.end()) return textures.at(path);

        textures[path] = IMG_LoadTexture(sdl_renderer, path.c_str());

        if (!textures[path])
            std::cerr << "TextureManager::LoadTexture(): Could not load texture from file: " << path << "! Returning nullptr\n";

        SDL_SetTextureScaleMode(textures[path], SDL_SCALEMODE_NEAREST);

        return textures[path];
    }


private:
    unordered_map<string, SDL_Texture*> textures;

    static inline SDL_Renderer* sdl_renderer;
};