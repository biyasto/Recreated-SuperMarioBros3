#include "Mushroom.h"
#include "Mario.h"
#include "Enemy.h"
#define LEFT_SIZE	48
#define LEAF_GRAVITY	0.0055f


Mushroom::Mushroom(Vector2 initPos) : CLeaf(initPos) {
	PowerUp = false;
}
Mushroom::Mushroom(Vector2 initPos,bool isPowerUp) : CLeaf(initPos) {
	PowerUp = isPowerUp;
}

void Mushroom::Render(Vector2 finalPos) {
	if(PowerUp==false)
		CAnimations::GetInstance()->Get("ani-1-up-mushroom")->Render(finalPos, Vector2(-nx, ny), 255);
	else CAnimations::GetInstance()->Get("ani-super-mushroom")->Render(finalPos, Vector2(-nx, ny), 255);
}

void Mushroom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	vy += LEAF_GRAVITY * dt;
	vx = 0.08f;
	if (vy > vyMax) vy = vyMax;
	CGameObject::Update(dt, coObjects);
	UpdateWithCollision(coObjects);	
}


void Mushroom::Collided(LPGAMEOBJECT obj) {
}

