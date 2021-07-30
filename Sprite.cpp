#include "Sprite.h"
#include "Game.h"
#include "debug.h"

CSprite::CSprite(std::string id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex)
{
	this->id = id;
	this->left = left;
	this->top = top;
	this->right = right;
	this->bottom = bottom;
	pivot.x = 0;
	pivot.y = 0;
	this->texture = tex;
}


CSprite::CSprite(std::string id, int left, int top, int right, int bottom, int xPivot, int yPivot, LPDIRECT3DTEXTURE9 tex)
{
	this->id = id;
	this->left = left;
	this->top = top;
	this->right = right;
	this->bottom = bottom;
	pivot = Vector2(xPivot, yPivot);
	this->texture = tex;
}





//void CSprite::Draw(Vector2 finalPos, int alpha)
//{
//	CGame* game = CGame::GetInstance();
//	RECT r;
//	r.left = left; r.right = right; r.top = top; r.bottom = bottom;
//	game->Draw(finalPos, pivot, texture, r, alpha);
//}

void CSprite::DrawWithScaling(Vector2 finalPos, Vector2 scale, int alpha)
{
	CGame* game = CGame::GetInstance();
	RECT r;
	r.left = left; r.right = right; r.top = top; r.bottom = bottom;
	
	

	game->DrawWithScaling(finalPos, pivot, texture, r, alpha, scale);
}

Vector2 CSprite::getSize() {
	return Vector2(
		this->right - this->left,
		this->bottom - this->top
	);
}

void CSprite::getSize(int& width, int& height) {
	width = this->right - this->left;
	height = this->bottom - this->top;
}