#include "raylib.h"

#include "../include/2DRenderer.hpp"
#include "../include/MM.hpp"
#include "../include/dataStructures.hpp"
#include <algorithm>
#include <unordered_map>
#include <vector>
#include <cstring>
#include "../include/lua_manager.hpp"


std::vector<TextureType*> textures;
std::vector<Tile*> tilesToRender;
std::unordered_map<int, Tile*> tiles;
std::vector<UIObject*> uiToRender;
std::unordered_map<int, UIObject*> ui;
Font fontToUse;

Texture2D loadSTexture(const char* filePath) {
    for (auto& i : textures) {
        if (strcmp(i->filePath, filePath) == 0) {
            return i->texture;
        };
    };
    Image image = LoadImage(filePath);
    TextureType* newTextureType = new TextureType();
    newTextureType->texture = LoadTextureFromImage(image);
    newTextureType->filePath = filePath;
    textures.push_back(newTextureType);
    UnloadImage(image);
    return newTextureType->texture;
};


void InitializeGame(int width, int height, const char* title) {
    InitWindow(width, height, title);
    fontToUse = LoadFontEx("resources/Determination.ttf", 64, 0, 0);

    Texture2D wabbit_alpha = loadSTexture("resources/wabbit_alpha.png");


    Player player;
    player.image = wabbit_alpha;
    player.zIndex = 2;
    player.position = (Vector2) {0,0};
    player.opacity = 255;

    Camera2D camera = { 0 };
    camera.target = player.position;
    camera.offset = (Vector2){ width/2.0f, height/2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    Tile* playerTile = new Tile();
    playerTile->image = player.image;
    playerTile->position = player.position;
    playerTile->zIndex = player.zIndex;
    playerTile->visible = true;
    playerTile->id = 1;


    tilesToRender.push_back(playerTile);
    tiles.emplace(playerTile->id, playerTile);

    initLua(&player);
    while (!WindowShouldClose())
	{
        // Sorts the z-index every frame (in case of new additions)
        playerTile->image = player.image;
        playerTile->position = player.position;
        playerTile->zIndex = player.zIndex;
        playerTile->opacity = player.opacity;
        playerTile->width = player.width;
        playerTile->height = player.height;
        playerTile->rotation = player.rotation;
        sortZIndexArray(tilesToRender);
        sortZIndexArrayUI(uiToRender);
        //camera.target = player.position;
        doLua();
		BeginDrawing();
            ClearBackground(WHITE);
            BeginMode2D(camera);
                RenderTiles(tilesToRender);
                RenderUI(uiToRender);
            EndMode2D();
		EndDrawing();
	};
    closeLua();
    FreeAllTextures();
};

// Sorts z-index array so that the order of which textures are drawn onto the screen are correct.
void sortZIndexArray(std::vector<Tile*>& array) {
    std::sort(array.begin(), array.end(), [](const Tile* a, const Tile* b) {
        return a->zIndex < b->zIndex;
    });
};

void sortZIndexArrayUI(std::vector<UIObject*>& array) {
    std::sort(array.begin(), array.end(), [](const UIObject* a, const UIObject* b) {
        return a->zIndex < b->zIndex;
    });
};


// Renders tiles onto the screen
void RenderTiles(std::vector<Tile*>& array) {
    for (int i = 0; i < array.size(); i++) {
        if (array.at(i)->visible == false) {
            continue;
        };
        unsigned char alpha = (unsigned char)(array.at(i)->opacity);
        DrawTexturePro(array.at(i)->image, {0, 0, 64, 64}, {array.at(i)->position.x, array.at(i)->position.y, array.at(i)->width+64, array.at(i)->height+64}, {32, 32}, array.at(i)->rotation, (Color){255, 255, 255, alpha});
    };
};

void RenderUI(std::vector<UIObject*>& array) {
    for (int i = 0; i < array.size(); i++) {
        if (array.at(i)->visible == false) {
            continue;
        };
        unsigned char alpha = (unsigned char)(array.at(i)->opacity);
        Vector2 origin = { array.at(i)->width, array.at(i)->height};

        DrawTexturePro(array.at(i)->image, 
        {0, 0, static_cast<float>(array.at(i)->image.width), static_cast<float>(array.at(i)->image.height)}, 
        {array.at(i)->position.x, array.at(i)->position.y, array.at(i)->width, array.at(i)->height}, 
        {array.at(i)->width/2,array.at(i)->height/2}, 
        array.at(i)->rotation, 
        (Color){255, 255, 255, alpha}
        );

        if (strcmp(array.at(i)->text, "") != 0) { 
            Vector2 textSize = MeasureTextEx(fontToUse, array.at(i)->text, array.at(i)->fontSize, 0);
            Vector2 textOrigin = { textSize.x/2, textSize.y/2};
            // Calculate the origin based on the text's size.
            if (strcmp(array.at(i)->textAlignX, "left") == 0) {
                textOrigin.x = 0;
            } else if (strcmp(array.at(i)->textAlignX, "right") == 0){
                textOrigin.x = textSize.x;
            };
            if (strcmp(array.at(i)->textAlignY, "up") == 0) {
                textOrigin.y = 0;
            } else if (strcmp(array.at(i)->textAlignY, "down") == 0){
                textOrigin.y = textSize.y;
            };

            // Adjust the position to the center of the UI object.
            Vector2 textPos = {
                array.at(i)->position.x - array.at(i)->offsetWidth + (array.at(i)->padding.left + array.at(i)->padding.right),
                array.at(i)->position.y + array.at(i)->offsetHeight + (array.at(i)->padding.top + array.at(i)->padding.bottom)
            };
            DrawTextPro(fontToUse, array.at(i)->text, textPos, textOrigin, array.at(i)->rotation, array.at(i)->fontSize, 0, array.at(i)->fontColor);
        }
    };
};

// This adds a the tile struct to the array. The tile struct contains the texture, position, and zIndex, will probably add more later.
void AddTexture(std::vector<Tile> *array, Texture texture, Vector2 position, int zIndex) {
    Tile newTile;
    newTile.image = texture;
    newTile.position = position;
    newTile.zIndex = zIndex;
    array->push_back(newTile);
};

// Frees all the textures inside of the tiles inside of the array, then clears the array. Used when the player leaves.
void FreeAllTextures() {
    for (auto& texture: textures) {
        UnloadTexture(texture->texture);
        delete texture;
    };
    textures.clear();
    for (std::unordered_map<int, Tile*>::iterator it = tiles.begin(); it != tiles.end(); ++it) {
        delete it->second; // Free the memory
    };
    for (auto& i: tilesToRender) {
        delete i;
    };
    for (std::unordered_map<int, UIObject*>::iterator it = ui.begin(); it != ui.end(); ++it) {
        delete it->second; // Free the memory
    };
    for (auto& i: uiToRender) {
        delete i;
    };
    tilesToRender.clear();
    tiles.clear();
};


Vector2 AddTwoVectors(Vector2 firstVector, Vector2 secondVector) {
    return (Vector2) {firstVector.x + secondVector.x, firstVector.y + secondVector.y};
};