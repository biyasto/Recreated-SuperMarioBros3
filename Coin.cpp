#include "Coin.h"
#include "AddingPointEffect.h"
#include "PlayScene.h"
#include "Mario.h"


CCoin::CCoin(Vector2 initPos) {
	this->x = initPos.x;
	this->y = initPos.y;
	allowOthersGoThrough = true;
	isDisable = false;
}

void CCoin::BeingCollided(LPGAMEOBJECT obj) {
	if (isDisable) return;
	if (dynamic_cast<CMario*>(obj)) {
		isDisable = true;
	}
}
void CCoin::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {

	CGameObject::Update(dt);
	UpdateWithCollision(coObjects);
}


void CCoin::Render(Vector2 finalPos) {
	CAnimations::GetInstance()->Get("ani-coin")->Render(finalPos, Vector2(-nx, ny), 255);
}



void CCoin::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	if (isDisable) {
		left = top = right = bottom = 0;
		return;
	}
	left = x - COIN_SIZE / 2;
	top = y - COIN_SIZE / 2;
	right = x + COIN_SIZE / 2;
	bottom = y + COIN_SIZE / 2;
}