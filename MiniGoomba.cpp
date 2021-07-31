
#include "MiniGoomba.h"

CMiniGoomba::CMiniGoomba(float x, float y, int nx) {
	this->x = x;
	this->y = y;
	this->nx = nx;
	this->isDisable = true;
	owner = NULL;
}

void CMiniGoomba::PrepareForShooting() {
	vy = 0.14;
}

void CMiniGoomba::UpdatePos(Vector2 pos, int nx) {
	this->x = pos.x;
	this->y = pos.y + 0;
	this->nx = nx;
}


void CMiniGoomba::CollidedLeftRight(LPGAMEOBJECT obj) {

	CGameObject::CollidedLeftRight(obj);
}



void CMiniGoomba::CollidedTop(LPGAMEOBJECT obj) {
	CGameObject::CollidedTop(obj);

}

void CMiniGoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	if (isDisable) return;

		vx = 0.0f;
		vy = 0.3f;

	CGameObject::Update(dt, coObjects);
	UpdateWithCollision(coObjects);

}

void CMiniGoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - 20 / 2;
	top = y - 20 / 2;
	right = x + 20 / 2;
	bottom = y + 20 / 2;

}



void CMiniGoomba::Render(Vector2 finalPos) {
	CAnimations::GetInstance()->Get("ani-tiny-goomba-fall")
		->Render(finalPos, Vector2(1, 1), 255);
}