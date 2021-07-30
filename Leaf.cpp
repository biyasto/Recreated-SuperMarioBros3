#include "Leaf.h"
#include "Mathf.h"
#include "Mario.h"
#include "PlayScene.h"


#define LEFT_SIZE	48
#define LEAF_GRAVITY	0.00015f
#define MUSHROOM 0.095f
#define LEAF_SPEED	    0.003f
#define LEAF_AMPLITUDE	60 

CLeaf::CLeaf(Vector2 initPos) {
	this->x = initPos.x;
	this->y = initPos.y;
	vx = vy = 0;
	initPosition = initPos;
	beginFalling = GetTickCount64();
	isTemp = true;

	type = ((CPlayScene*)(CGame::GetInstance()->GetCurrentScene()))->GetMarioType();
	if (type == MarioType::TALL) vy = -0.1f; 
	
}

void CLeaf::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	

	switch(type)
	{
	
	case MarioType::TALL	:BehaviorLeaf(dt, coObjects); break;
	case MarioType::TAIL:	BehaviorFlower(dt, coObjects); break; //BehaviorFlower(dt, coObjects); break;
	case MarioType::FIRE:	BehaviorFlower(dt, coObjects); break;
	default:BehaviorLeaf(dt, coObjects); break;
	}
	

}

void CLeaf::BehaviorLeaf(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	vy += LEAF_GRAVITY * dt;
	if (vy > vyMax) vy = vyMax;
	CGameObject::Update(dt, coObjects);
	UpdateWithCollision(coObjects);
	float time = GetTickCount64() - beginFalling;
	if (vy > 0)
		x = initPosition.x + LEAF_AMPLITUDE * cos(LEAF_SPEED * time * 1.0f - Mathf::Pi / 2);
}
void CLeaf::BehaviorFlower(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {

	UpdateWithCollision(coObjects);
}

void CLeaf::Render(Vector2 finalPos) {

	switch (type)
	{
	
	case MarioType::TALL: CAnimations::GetInstance()->Get("ani-super-leaf-red")->Render(finalPos, Vector2(-nx, ny), 255); break;
	case MarioType::TAIL: CAnimations::GetInstance()->Get("ani-fire-flower")->Render(finalPos, Vector2(-nx, ny), 255);break;
	case MarioType::FIRE: CAnimations::GetInstance()->Get("ani-fire-flower")->Render(finalPos, Vector2(-nx, ny), 255); break;
		;
	}
	
}



void CLeaf::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	left = x - LEFT_SIZE / 2;
	top = y - LEFT_SIZE / 2;
	right = x + LEFT_SIZE / 2;
	bottom = y + LEFT_SIZE / 2;
}

void CLeaf::Collided(LPGAMEOBJECT obj) {
	
	if (dynamic_cast<CMario*>(obj)) {
		x = 0;y = 0;
	((CMario*)(obj))->y -= 15;
	((CMario*)(obj))->ChangeAction(MarioAction::UPGRADE_LV);
	isDisable = true;
	}

}