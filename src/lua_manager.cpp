#include <cstring>
#include <unordered_map>
#include <algorithm>
#include <vector>
#include "../include/dataStructures.hpp"
#include "../include/GetKeyFromString.cpp"
#include "../include/types.hpp"
#include "../include/lua_manager.hpp"
#include "../include2/lua.hpp"

lua_State* Ls = luaL_newstate();
int id = 2;
int uiid = 1;

// INIT FUNCTIONS

void initLua(Player* player) {
    registerPlayer(Ls);
    registerPlayerUserData(Ls, player);
    lua_setglobal(Ls, "player");

    registerObjectMetatable(Ls);
    registerUIMetatable(Ls);

    luaL_openlibs(Ls);
    lua_register(Ls, "IsKeyDown", luaIsKeyDown);
    lua_register(Ls, "IsKeyPressed", luaIsKeyPressed);
    lua_register(Ls, "object", registerNewObject);
    lua_register(Ls, "deleteObject", deleteObject);
    lua_register(Ls, "clearObjects", clearAllObjects);
    lua_register(Ls, "deleteUI", deleteUI);
    lua_register(Ls, "clearUI", clearAllUI);
    lua_register(Ls, "ui", registerNewUI);


    luaL_dofile(Ls, "src/main.lua");
};

void doLua() {
    float deltaTime = GetFrameTime();
    lua_getglobal(Ls, "update");
    if (lua_isfunction(Ls, -1)) {
        lua_pushnumber(Ls, deltaTime);
        if (lua_pcall(Ls, 1, 0, 0) != LUA_OK) {
            TraceLog(LOG_ERROR, "Lua Error: %s", lua_tostring(Ls, -1));
            lua_pop(Ls, 1);
        }
    } else {
        lua_pop(Ls, 1);
    }    
};

void closeLua() {
    lua_close(Ls);
};

// GENERAL FUNCTIONS

int luaIsKeyDown(lua_State* L) {
    const char* keyName = luaL_checkstring(L, 1);
    int key = GetKeyFromString(keyName);
    if (key == 0) {
        TraceLog(LOG_ERROR, "Invalid key name: %s", keyName);
        return 0;
    }
    lua_pushboolean(L, IsKeyDown(key));
    return 1;
};

int luaIsKeyPressed(lua_State* L) {
    const char* keyName = luaL_checkstring(L, 1);
    int key = GetKeyFromString(keyName);
    if (key == 0) {
        TraceLog(LOG_ERROR, "Invalid key name: %s", keyName);
        return 0;
    };
    lua_pushboolean(L, IsKeyPressed(key));
    return 1;
}

int deleteObject(lua_State* L) {
    int ids = luaL_checkinteger(L, 1);
    auto it = tiles.find(ids);
    if (it != tiles.end()) {
        delete it->second;
        tiles.erase(it);
    }
    tilesToRender.erase(
        std::remove_if(
            tilesToRender.begin(),
            tilesToRender.end(),
            [ids](Tile* tile) {
                if (tile->id == ids) {
                    delete tile;
                    return true;
                }
                return false;
            }
        ),
        tilesToRender.end()
    );
    return 0;
};

int clearAllObjects(lua_State* L) {
    for (std::unordered_map<int, Tile*>::iterator it = tiles.begin(); it != tiles.end(); ++it) {
        delete it->second; // Free the memory
    }
    for (auto& i: tilesToRender) {
        delete i;
    };
    tiles.clear();
    tilesToRender.clear();
    return 0;
};

int deleteUI(lua_State* L) {
    int ids = luaL_checkinteger(L, 1);
    auto it = ui.find(ids);
    if (it != ui.end()) {
        delete it->second;
        ui.erase(it);
    }
    uiToRender.erase(
        std::remove_if(
            uiToRender.begin(),
            uiToRender.end(),
            [ids](UIObject* uiTile) {
                if (uiTile->id == ids) {
                    delete uiTile;
                    return true;
                }
                return false;
            }
        ),
        uiToRender.end()
    );
    return 0;
};

int clearAllUI(lua_State* L) {
    for (std::unordered_map<int, UIObject*>::iterator it = ui.begin(); it != ui.end(); ++it) {
        delete it->second; // Free the memory
    }
    for (auto& i: uiToRender) {
        delete i;
    };
    ui.clear();
    uiToRender.clear();
    return 0;
};

// REGISTER FUNCTIONS

void registerPlayer(lua_State* L) {
    luaL_newmetatable(L, "PlayerMetatable");

    lua_pushstring(L, "__index");
    lua_pushcfunction(L, playerGetField);
    lua_settable(L, -3);

    lua_pushstring(L, "__newindex");
    lua_pushcfunction(L, playerSetField);
    lua_settable(L, -3);

    lua_pop(L, 1);
};

void registerPlayerUserData(lua_State* L, Player* player) {
    Player** userData = (Player**)lua_newuserdata(L, sizeof(Player*));
    *userData = player;

    luaL_getmetatable(L, "PlayerMetatable");
    lua_setmetatable(L, -2);
};

void registerObjectMetatable(lua_State* L) {
    luaL_newmetatable(L, "ObjectMetatable");

    lua_pushstring(L, "__index");
    lua_pushcfunction(L, objectGetField);
    lua_settable(L, -3);

    lua_pushstring(L, "__newindex");
    lua_pushcfunction(L, objectSetField);
    lua_settable(L, -3);

    lua_pop(L, 1);
};

int registerNewObject(lua_State* L) {

    Tile* newTile = new Tile();
    newTile->id = id;
    newTile->opacity = 255;
    newTile->visible = true;

    tiles.emplace(id, newTile);
    tilesToRender.push_back(newTile);

    lua_newtable(L);

    lua_pushstring(L, "id");
    lua_pushinteger(L, id);
    lua_settable(L, -3);

    luaL_getmetatable(L, "ObjectMetatable");
    lua_setmetatable(L, -2);
    id++;
    return 1;
};

void registerUIMetatable(lua_State* L) {
    luaL_newmetatable(L, "UIMetatable");

    lua_pushstring(L, "__index");
    lua_pushcfunction(L, uiGetField);
    lua_settable(L, -3);

    lua_pushstring(L, "__newindex");
    lua_pushcfunction(L, uiSetField);
    lua_settable(L, -3);

    lua_pop(L, 1);
};

int registerNewUI(lua_State* L) {
    UIObject* newUI = new UIObject();
    newUI->id = uiid;
    newUI->opacity = 255;
    newUI->visible = true;
    newUI->fontSize = 64;
    newUI->fontColor = (Color) {255, 255, 255, 255};
    newUI->text = "";
    newUI->height = 64;
    newUI->width = 64;

    ui.emplace(uiid, newUI);
    uiToRender.push_back(newUI);

    lua_newtable(L);

    lua_pushstring(L, "id");
    lua_pushinteger(L, uiid);
    lua_settable(L, -3);

    luaL_getmetatable(L, "UIMetatable");
    lua_setmetatable(L, -2);
    uiid++;
    return 1;
};

// __INDEX AND __NEWINDEX FUNCTIONS

int playerGetField(lua_State* L) {
    Player* player = *(Player**)luaL_checkudata(L, 1, "PlayerMetatable");
    const char * field = luaL_checkstring(L, 2);

    if (strcmp(field, "x") == 0) {
        lua_pushnumber(L, player->position.x);
    } else if (strcmp(field, "y") == 0) {
        lua_pushnumber(L, player->position.y);
    } else if (strcmp(field, "zIndex") == 0) {
        lua_pushnumber(L, player->zIndex);
    } else if (strcmp(field, "image") == 0) {
        for (auto& i: textures) {
            if (i->texture.id == player->image.id) {
                lua_pushstring(L, i->filePath);
            };
        };
    } else if (strcmp(field, "position") == 0) {
        lua_pushstring(L, "Position currently not implemented");
    } else if (strcmp(field, "opacity") == 0) {
        lua_pushinteger(L, player->opacity);
        return 1;
    } else if (strcmp(field, "width") == 0) {
        lua_pushnumber(L, player->width);
        return 1;
    } else if (strcmp(field, "height") == 0) {
        lua_pushnumber(L, player->height);
        return 1;
    } else if (strcmp(field, "rotation") == 0) {
        lua_pushnumber(L, player->rotation);
        return 1;
    };

    return 1;
};

int playerSetField(lua_State* L) {
    Player* player = *(Player**)luaL_checkudata(L, 1, "PlayerMetatable");
    const char * field = luaL_checkstring(L, 2);
    if (strcmp(field, "x") == 0) {
        player->position.x = luaL_checknumber(L, 3);
    } else if (strcmp(field, "y") == 0) {
        player->position.y = luaL_checknumber(L, 3);
    } else if (strcmp(field, "zIndex") == 0) {
        player->zIndex = luaL_checknumber(L, 3);
    } else if (strcmp(field, "image") == 0) {
        player->image = loadSTexture(luaL_checkstring(L, 3));
    } else if (strcmp(field, "opacity") == 0) {
        player->opacity = luaL_checkinteger(L, 3);
        return 1;
    } else if (strcmp(field, "width") == 0) {
        player->width = luaL_checknumber(L, 3);
        return 1;
    } else if (strcmp(field, "height") == 0) {
        player->height = luaL_checknumber(L, 3);
        return 1;
    } else if (strcmp(field, "rotation") == 0) {
        player->rotation = luaL_checknumber(L, 3);
        return 1;
    };

    return 0;
}

int objectGetField(lua_State* L) {
    const char* field = luaL_checkstring(L, 2);

    // Retrieve the "id" field
    lua_getfield(L, 1, "id");
    if (!lua_isinteger(L, -1)) {
        return luaL_error(L, "Object has no valid 'id' field.");
    }
    int localID = lua_tointeger(L, -1);
    lua_pop(L, 1); // Clean up stack

    // Retrieve Tiles map from userdata
    auto it = tiles.find(localID);
    // Handle fields like "x"
    if (strcmp(field, "x") == 0) {
        lua_pushnumber(L, it->second->position.x);
        return 1;
    } else if (strcmp(field, "y") == 0) {
        lua_pushnumber(L, it->second->position.y);
        return 1;
    } else if (strcmp(field, "image") == 0) {
        for (auto& i: textures) {
            if (i->texture.id == it->second->image.id) {
                lua_pushstring(L, i->filePath);
                return 1;
            };
        };
        lua_pushstring(L, "Unable to find image.");
        return 1;
    } else if (strcmp(field, "zIndex") == 0) {
        lua_pushnumber(L, it->second->zIndex);
        return 1;
    } else if (strcmp(field, "visible") == 0) {
        lua_pushboolean(L, it->second->visible);
        return 1;
    } else if (strcmp(field, "opacity") == 0) {
        lua_pushinteger(L, it->second->opacity);
        return 1;
    } else if (strcmp(field, "width") == 0) {
        lua_pushnumber(L, it->second->width);
        return 1;
    } else if (strcmp(field, "height") == 0) {
        lua_pushnumber(L, it->second->height);
        return 1;
    } else if (strcmp(field, "rotation") == 0) {
        lua_pushnumber(L, it->second->rotation);
        return 1;
    };
    return luaL_error(L, "Unknown field: %s", field);
};

int objectSetField(lua_State* L) {
    const char* field = luaL_checkstring(L, 2);

    // Retrieve the "id" field
    lua_getfield(L, 1, "id");
    if (!lua_isinteger(L, -1)) {
        return luaL_error(L, "Object has no valid 'id' field.");
    }
    int localID = lua_tointeger(L, -1);
    lua_pop(L, 1); // Clean up stack

    // Retrieve Tiles map from userdata
    auto it = tiles.find(localID);

    Tile* tile = it->second;
    // Handle fields like "x"
    if (strcmp(field, "x") == 0) {
        tile->position.x = luaL_checknumber(L, 3);
        return 0;
    } else if (strcmp(field, "y") == 0) {
        tile->position.y = luaL_checknumber(L, 3);
        return 0;
    } else if (strcmp(field, "image") == 0) {
        tile->image = loadSTexture(luaL_checkstring(L, 3));
        return 0;
    } else if (strcmp(field, "zIndex") == 0) {
        tile->zIndex = luaL_checknumber(L, 3);
        return 0;
    } else if (strcmp(field, "visible") == 0) {
        tile->visible = lua_toboolean(L, 3);
        return 0;
    } else if (strcmp(field, "opacity") == 0) {
        tile->opacity = luaL_checkinteger(L, 3);
        return 1;
    } else if (strcmp(field, "width") == 0) {
        tile->width = luaL_checknumber(L, 3);
        return 1;
    } else if (strcmp(field, "height") == 0) {
        tile->height = luaL_checknumber(L, 3);
        return 1;
    } else if (strcmp(field, "rotation") == 0) {
        tile->rotation = luaL_checknumber(L, 3);
        return 1;
    };
    return luaL_error(L, "Unknown field: %s", field);
};

int uiGetField(lua_State* L) {
    const char* field = luaL_checkstring(L, 2);

    // Retrieve the "id" field
    lua_getfield(L, 1, "id");
    if (!lua_isinteger(L, -1)) {
        return luaL_error(L, "Object has no valid 'id' field.");
    }
    int localID = lua_tointeger(L, -1);
    lua_pop(L, 1); // Clean up stack

    auto it = ui.find(localID);
    // Handle fields like "x"
    if (strcmp(field, "x") == 0) {
        lua_pushnumber(L, it->second->position.x);
        return 1;
    } else if (strcmp(field, "y") == 0) {
        lua_pushnumber(L, it->second->position.y);
        return 1;
    } else if (strcmp(field, "image") == 0) {
        for (auto& i: textures) {
            if (i->texture.id == it->second->image.id) {
                lua_pushstring(L, i->filePath);
                return 1;
            };
        };
        lua_pushstring(L, "Unable to find image.");
        return 1;
    } else if (strcmp(field, "zIndex") == 0) {
        lua_pushnumber(L, it->second->zIndex);
        return 1;
    } else if (strcmp(field, "visible") == 0) {
        lua_pushboolean(L, it->second->visible);
        return 1;
    } else if (strcmp(field, "opacity") == 0) {
        lua_pushinteger(L, it->second->opacity);
        return 1;
    } else if (strcmp(field, "width") == 0) {
        lua_pushnumber(L, it->second->width);
        return 1;
    } else if (strcmp(field, "height") == 0) {
        lua_pushnumber(L, it->second->height);
        return 1;
    } else if (strcmp(field, "rotation") == 0) {
        lua_pushnumber(L, it->second->rotation);
        return 1;
    } else if (strcmp(field, "text") == 0) {
        lua_pushstring(L, it->second->text);
        return 1;
    } else if (strcmp(field, "fontSize") == 0) {
        lua_pushnumber(L, it->second->fontSize);
        return 1;
    } else if (strcmp(field, "fontColor") == 0) {
        lua_newtable(L);
        lua_pushstring(L, "r");
        lua_pushnumber(L, it->second->fontColor.r);
        lua_settable(L, -3);

        lua_pushstring(L, "g");
        lua_pushnumber(L, it->second->fontColor.g);
        lua_settable(L, -3); 

        lua_pushstring(L, "b");
        lua_pushnumber(L, it->second->fontColor.b);
        lua_settable(L, -3);

        lua_pushstring(L, "a");
        lua_pushnumber(L, it->second->fontColor.a);
        lua_settable(L, -3);
        
        return 1;
    } else if (strcmp(field, "widthCO") == 0) {
        lua_pushnumber(L, it->second->widthCO);
        return 1;
    };
    return luaL_error(L, "Unknown field: %s", field);
};

int uiSetField(lua_State* L) {
    const char* field = luaL_checkstring(L, 2);

    // Retrieve the "id" field
    lua_getfield(L, 1, "id");
    if (!lua_isinteger(L, -1)) {
        return luaL_error(L, "Object has no valid 'id' field.");
    }
    int localID = lua_tointeger(L, -1);
    lua_pop(L, 1); // Clean up stack

    auto it = ui.find(localID);

    UIObject* uiTile = it->second;
    // Handle fields like "x"
    if (strcmp(field, "x") == 0) {
        uiTile->position.x = luaL_checknumber(L, 3);
        return 0;
    } else if (strcmp(field, "y") == 0) {
        uiTile->position.y = luaL_checknumber(L, 3);
        return 0;
    } else if (strcmp(field, "image") == 0) {
        uiTile->image = loadSTexture(luaL_checkstring(L, 3));
        uiTile->baseWidth = uiTile->image.width/4;
        uiTile->baseHeight = uiTile->image.height/4;
        return 0;
    } else if (strcmp(field, "zIndex") == 0) {
        uiTile->zIndex = luaL_checknumber(L, 3);
        return 0;
    } else if (strcmp(field, "visible") == 0) {
        uiTile->visible = lua_toboolean(L, 3);
        return 0;
    } else if (strcmp(field, "opacity") == 0) {
        uiTile->opacity = luaL_checkinteger(L, 3);
        return 0;
    } else if (strcmp(field, "width") == 0) {
        uiTile->width = luaL_checknumber(L, 3);
        uiTile->widthCO = uiTile->width/2;
        return 0;
    } else if (strcmp(field, "height") == 0) {
        uiTile->height = luaL_checknumber(L, 3);
        if (uiTile->height != uiTile->baseHeight*4) {
            uiTile->heightCO = uiTile->height/4;
        } else {
            uiTile->heightCO = 0;
        };
        return 0;
    } else if (strcmp(field, "rotation") == 0) {
        uiTile->rotation = luaL_checknumber(L, 3);
        return 0;
    } else if (strcmp(field, "text") == 0) {
        uiTile->text = luaL_checkstring(L, 3);
        return 0;
    } else if (strcmp(field, "fontSize") == 0) {
        uiTile->fontSize = luaL_checknumber(L, 3);
        return 0;
    } else if (strcmp(field, "fontColor") == 0) {
        lua_pushstring(L, "r");
        lua_gettable(L, 3);
        uiTile->fontColor.r = lua_tonumber(L, -1);
        lua_pop(L, 1);

        lua_pushstring(L, "g");
        lua_gettable(L, 3);
        uiTile->fontColor.g = lua_tonumber(L, -1);
        lua_pop(L, 1);

        lua_pushstring(L, "b");
        lua_gettable(L, 3);
        uiTile->fontColor.b = lua_tonumber(L, -1);
        lua_pop(L, 1);

        lua_pushstring(L, "a");
        lua_gettable(L, 3);
        uiTile->fontColor.a = lua_tonumber(L, -1);
        lua_pop(L, 1);
        return 0;
    };
    return luaL_error(L, "Unknown field: %s", field);
};