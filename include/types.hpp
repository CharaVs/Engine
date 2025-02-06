#pragma once
#include "raylib.h"
#include <vector>

typedef struct Player {
    Vector2 position;
    bool canJump;
    float movementSpeed;
    Texture2D image;
    float width;
    float height;
    float rotation;
    int opacity;
    int zIndex;
} Player;


typedef struct Tile {
    Vector2 position;
    Texture2D image;
    float width;
    float height;
    float rotation;
    int zIndex;
    bool visible;
    int opacity;
    int id;
} Tile;

typedef struct TextureType {
    Texture2D texture;
    const char* filePath;
} TextureType;

typedef struct UIObject {
    Vector2 position;
    Texture2D image;
    int zIndex;
    bool visible;
    float width;
    float height;
    float rotation;
    const char * text;
    int fontSize;
    Color fontColor;
    int baseWidth;
    int baseHeight;
    int widthCO;
    int heightCO;
    int opacity;
    int id;
} UIObject;
