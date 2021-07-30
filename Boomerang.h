#pragma once
#include "GameObject.h"

class CBoomerang : public CGameObject
{
	Vector2 venusBulletDirection = Vector2(1, 1);
	LPGAMEOBJECT owner;
public:
	CBoomerang(float x, float y, int nx);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render(Vector2 finalPos);

	void PrepareForShooting();
	void UpdatePos(Vector2 pos, int nx = 1);
	void SetVenusBulletDirection(Vector2 a) { venusBulletDirection = a; }
	void SetOwner(LPGAMEOBJECT obj) { owner = obj; }
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	virtual void CollidedLeftRight(LPGAMEOBJECT) override;
	virtual void CollidedTop(LPGAMEOBJECT) override;
	virtual void CollidedBottom(LPGAMEOBJECT) override {};
	virtual void Collided(LPGAMEOBJECT) override {};
};

