#pragma once

#include "Sprite.h"
#include "TextureManager.h"

/*
	Manage sprite database
*/
class CSprites
{
	static CSprites* __instance;

	std::unordered_map<std::string, LPSPRITE> sprites;

public:
	void Add(std::string id, int left, int top, int right, int bottom, int xPivot=0, int yPivot=0, LPDIRECT3DTEXTURE9 tex=NULL);
	LPSPRITE Get(std::string id);
	void Clear();

	bool LoadSpriteFromFile(std::string filePath);

	static CSprites* GetInstance();
};