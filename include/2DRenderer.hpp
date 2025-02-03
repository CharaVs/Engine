#pragma once
#include <vector>
#include "types.hpp"

void InitializeGame(int width, int height, const char* title);
void RenderTiles(std::vector<Tile*>& array);
void RenderUI(std::vector<UIObject*>& array);