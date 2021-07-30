#include "TailAttackBox.h"
#include "Enemy.h" 
#include "GoldenBrick.h"

CTailAttackBox::CTailAttackBox() {
	this->x = 0;
	this->y = 0;
}

void CTailAttackBox::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	if (!isOpening) {
		left	= 0; 
		top		= 0; 
		right	= 0;
		bottom	= 0;
		return;
	}
	else {
		left = this->x - size.x / 2;
		top = this->y - size.y / 2;
		right = this->x + size.x / 2;
		bottom = this->y + size.y / 2;
	}
}

void CTailAttackBox::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	vx = vy = 0;
	CGameObject::Update(dt);
	UpdateWithCollision(coObjects);
}
void CTailAttackBox::Render(Vector2 finalPos) {
	RenderBoundingBox(finalPos);
}
void CTailAttackBox::UpdatePosition(Vector2 newPos) {
	this->x = newPos.x;
	this->y = newPos.y;
}

void CTailAttackBox::OnHadCollided(LPGAMEOBJECT obj) {
	if (dynamic_cast<CEnemy*>(obj) || dynamic_cast<CGoldenBrick*>(obj)) {
		isOpening = false;
		hasAttacked = true;
	}

	if (dynamic_cast<CEnemy*>(obj))
		((CEnemy*)obj)->ChangeState(EEnemyState::ONESHOTDIE);
}
void CTailAttackBox::SetIsOpening(bool a) {
	if (a == false) isOpening = a;
	else if (!hasAttacked) isOpening = a;
}

