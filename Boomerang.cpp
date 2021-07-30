#include "Boomerang.h"


CBoomerang::CBoomerang(float x, float y, int nx) {
	this->x = x;
	this->y = y;
	this->nx = nx;
	this->isDisable = true;
	owner = NULL;
}

void CBoomerang::PrepareForShooting() {
	vy = 0.14;
}

void CBoomerang::UpdatePos(Vector2 pos, int nx) {
	this->x = pos.x;
	this->y = pos.y + 0;
	this->nx = nx;
}


void CBoomerang::CollidedLeftRight(LPGAMEOBJECT obj) {
	
	CGameObject::CollidedLeftRight(obj);
}



void CBoomerang::CollidedTop(LPGAMEOBJECT obj) {
	CGameObject::CollidedTop(obj);
	
}

void CBoomerang::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	
	if (isDisable) return;

	if (y <= owner->y) {
		vx += -.007f;
		vy += .005f;
		
	}
	else {
		vx = -.15f;
		vy = .0f;
	}


	CGameObject::Update(dt, coObjects);

	UpdateWithCollision(coObjects);

}

void CBoomerang::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - 46 / 2;
	top = y - 46 / 2;
	right = x + 46 / 2;
	bottom = y + 46 / 2;

}



void CBoomerang::Render(Vector2 finalPos) {
	CAnimations::GetInstance()->Get("ani-boomerang-spin")
		->Render(finalPos, Vector2(1, 1), 255);

}