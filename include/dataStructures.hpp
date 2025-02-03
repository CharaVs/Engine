#pragma once
#include "raylib.h"
#include <vector>
#include "types.hpp"
#include <unordered_map>



void AddTexture(std::vector<Tile*> *array, Texture texture, Vector2 position, int zIndex);
void RemoveTexture(std::vector<Tile*> *array, int index);
void FreeAllTextures();
void sortZIndexArray(std::vector<Tile*>& array);
void sortZIndexArrayUI(std::vector<UIObject*>& array);
std::vector<TextureType*> getTextures();
Texture2D loadSTexture(const char* filePath);
extern std::vector<Tile*> tilesToRender;
extern std::unordered_map<int, Tile*> tiles;
extern std::vector<TextureType*> textures;
extern std::vector<UIObject*> uiToRender;
extern std::unordered_map<int, UIObject*> ui;