#include "FireBullet.h"

CFireBullet::CFireBullet(float x, float y, int nx, int owner) {
	this->x = x;
	this->y = y;
	this->nx = nx;
	this->owner = owner;
	this->isDisable = true;
}

void CFireBullet::PrepareForShooting() {
	vy = VELOCITY_Y_FIRE_BULLET;
}

void CFireBullet::UpdatePos(Vector2 pos, int nx) {
	this->x = pos.x;
	this->y = pos.y + INITIAL_DELTA_Y_FIRE_BULLET;
	this->nx = nx;
}


void CFireBullet::CollidedLeftRight(LPGAMEOBJECT obj) {
	if(owner == 0)
		this->isDisable = true;
	CGameObject::CollidedLeftRight(obj);
}

void CFireBullet::OnHadCollided(LPGAMEOBJECT obj) {
	obj->BeingCollided(this);
}

void CFireBullet::CollidedTop(LPGAMEOBJECT obj) {
	CGameObject::CollidedTop(obj);
	if(owner == 0) vy = -VELOCITY_Y_FIRE_BULLET_BOUNCE;
}

void CFireBullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	if (isDisable) return;

	if (owner == 0) {
		vx = VELOCITY_X_FIRE_BULLET * nx * dt;
		ApplyGravity();
	}
	if (owner == 1) {
		vx = 0.07f * venusBulletDirection.x;
		vy = 0.05f * venusBulletDirection.y;
	}
	
	CGameObject::Update(dt, coObjects);

	UpdateWithCollision(coObjects);

}

void CFireBullet::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - FIRE_BULLET_WIDTH / 2;
	top = y - FIRE_BULLET_HEIGHT / 2;
	right = x + FIRE_BULLET_WIDTH / 2;
	bottom = y + FIRE_BULLET_HEIGHT /2;

}



void CFireBullet::Render(Vector2 finalPos) {
	CAnimations::GetInstance()->Get(ANI_FIRE_BULLET)
		->Render(finalPos, Vector2(nx,ny), 255);
	//RenderBoundingBox(finalPos);
}