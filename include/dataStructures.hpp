#pragma once
#include "raylib.h"
#include <vector>
#include "types.hpp"

void AddTexture(std::vector<Tile*> *array, Texture texture, Vector2 position, int zIndex);
void RemoveTexture(std::vector<Tile*> *array, int index);
void FreeAllTextures(std::vector<Tile*> *array);
void sortZIndexArray(std::vector<Tile*> array);