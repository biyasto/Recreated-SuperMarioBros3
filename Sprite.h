#pragma once
#include <Windows.h>
#include <d3dx9.h>
#include <unordered_map>
#include "Transform.h"

class CSprite
{
	std::string id;				

	int left;
	int top;
	int right;
	int bottom;
	Vector2 pivot = Vector2(0,0);

	LPDIRECT3DTEXTURE9 texture;
public:
	CSprite(std::string id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex);
	CSprite(std::string id, int left, int top, int right, int bottom, int xPivot, int yPivot, LPDIRECT3DTEXTURE9 tex);
	
	void getSize(int &width, int &height);
	Vector2 getSize();
	void DrawWithScaling(Vector2 finalPos, Vector2 = Vector2(1,1), int alpha = 255);

};

typedef CSprite* LPSPRITE;