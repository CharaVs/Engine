#pragma once
#include "raylib.h"
#include <cstring>
#include <unordered_map>
#include <algorithm>
#include "../include/types.hpp"
#include "../include2/lua.hpp"

// INIT FUNCTIONS

void initLua(Player* player);
void doLua();
void closeLua();

// GENERAL FUNCTIONS

int luaIsKeyDown(lua_State* L);
int luaIsKeyPressed(lua_State* L);
int deleteObject(lua_State* L);
int clearAllObjects(lua_State* L);
int deleteUI(lua_State* L);
int clearAllUI(lua_State* L);

// REGISTER FUNCTIONS

void registerPlayer(lua_State* L);
void registerPlayerUserData(lua_State* L, Player* player);

void registerObjectMetatable(lua_State* L);
int registerNewObject(lua_State* L);

void registerUIMetatable(lua_State* L);
int registerNewUI(lua_State* L);

// __INDEX and __NEWINDEX FUNCTIONS

int playerGetField(lua_State* L);
int playerSetField(lua_State* L);

int objectGetField(lua_State* L);
int objectSetField(lua_State* L);

int uiGetField(lua_State* L);
int uiSetField(lua_State* L);
