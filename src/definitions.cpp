#define SOL_ALL_SAFETIES_ON 1
#include "raylib.h"

#include "../include/2DRenderer.hpp"
#include "../include/MM.hpp"
#include "../include/dataStructures.hpp"
#include <algorithm>
#include "../include/Properties.hpp"
#include <cstring>
#include "C:/Users/krist/OneDrive/Desktop/Engine/include2/lua.hpp"
Texture2D textures = {};

Texture2D loadSTexture(const char* filePath) {
    Image image = LoadImage(filePath);
    Texture2D imageToTexture = LoadTextureFromImage(image);
    UnloadImage(image);
    return imageToTexture;
};

Property GetProperty(const char *name) {
    if (strcmp(name, "position")) {
        return POSITION;
    };
};

int newObject(lua_State* L) {

};

void InitializeGame(int width, int height, const char* title) {
    InitWindow(width, height, title);

    std::vector<Tile*> tilesToRender;

    Texture2D wabbit_alpha = loadSTexture("resources/wabbit_alpha.png");

    lua_State* L = luaL_newstate();

    luaL_openlibs(L);

    luaL_dofile(L, "src/main.lua");

    Player player;
    player.playerImage = wabbit_alpha;
    player.zIndex = 1;
    player.position = (Vector2) {0,0};

    Camera2D camera = { 0 };
    camera.target = player.position;
    camera.offset = (Vector2){ width/2.0f, height/2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    Tile* playerTile = new Tile();


    tilesToRender.push_back(playerTile);

    while (!WindowShouldClose())
	{
        // Sorts the z-index every frame (in case of new additions)
        playerTile->tileImage = player.playerImage;
        playerTile->position = player.position;
        playerTile->zIndex = player.zIndex;
        sortZIndexArray(tilesToRender);
        //camera.target = player.position;
        if (IsKeyPressed(KEY_W)) {
            player.position = AddTwoVectors(player.position, (Vector2) {50, 50});
        };
		BeginDrawing();
            ClearBackground(WHITE);
            BeginMode2D(camera);
                RenderTiles(&tilesToRender);
            EndMode2D();
		EndDrawing();
	};

    lua_close(L);
};

// Sorts z-index array so that the order of which textures are drawn onto the screen are correct.
void sortZIndexArray(std::vector<Tile*> array) {
    std::sort(array.begin(), array.end(), [](const Tile* a, const Tile* b) {
        return a->zIndex < b->zIndex;
    });
};

// Renders tiles onto the screen
void RenderTiles(std::vector<Tile*> *array) {
    for (int i = 0; i < array->size(); i++) {
        DrawTexture(array->at(i)->tileImage, array->at(i)->position.x, array->at(i)->position.y, WHITE);
    };
};

// This adds a the tile struct to the array. The tile struct contains the texture, position, and zIndex, will probably add more later.
void AddTexture(std::vector<Tile> *array, Texture texture, Vector2 position, int zIndex) {
    Tile newTile;
    newTile.tileImage = texture;
    newTile.position = position;
    newTile.zIndex = zIndex;
    array->push_back(newTile);
};

// Frees all the textures inside of the tiles inside of the array, then clears the array. Used when the player leaves.
void FreeAllTextures(std::vector<Tile> *array) {
    for (int i = 0; i < array->size(); i++) {
        UnloadTexture(array->at(i).tileImage);
    };
    array->clear();
};


Vector2 AddTwoVectors(Vector2 firstVector, Vector2 secondVector) {
    return (Vector2) {firstVector.x + secondVector.x, firstVector.y + secondVector.y};
};