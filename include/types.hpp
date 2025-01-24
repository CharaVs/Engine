#pragma once
#include "raylib.h"

typedef struct Player {
    Vector2 position;
    bool canJump;
    float movementSpeed;
    Texture2D playerImage;
    int zIndex;
} Player;

typedef struct Tile {
    Vector2 position;
    Texture2D tileImage;
    int zIndex;
} Tile;

