
#pragma once
#ifndef __GAME_MAP__
#define __GAME_MAP__

#include <d3dx9.h>
#include <d3d9.h>
#include <vector>
#include "TextureManager.h"
#include "Sprite.h"
#include "MapReader/Tmx.h.in"
#include "Utils.h"
#include "GameObject.h"

class GameMap
{
public:
    /*GameMap(const char* filePath, std::vector<LPGAMEOBJECT>* listObjects);*/
    GameMap(const char* filePath);
    Tmx::Map* GetMap();

    int GetWidth();
    int GetHeight();
    int GetTileWidth();
    int GetTileHeight();

    // Area of visible ( Camera )
    Vector2 camPosition;

    void UpdateCamPosition(Vector2 newPos);

    void Draw();

    ~GameMap();

private:
    void LoadMap(const char* filePath);
    //std::vector<LPGAMEOBJECT>* listObjects;
    std::list<int> listNoCollision = { 40 };
    Tmx::Map* mMap;
    std::vector<std::string> mListTilesetId;
    bool hasLoaded = false;
    bool isExistInList(int a);
    Vector2 ConvertToPositionInCam(Vector2 oldPos);

};

#endif

