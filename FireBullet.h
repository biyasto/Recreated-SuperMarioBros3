#pragma once
#include "GameObject.h"


#define FIRE_BULLET_WIDTH		30
#define FIRE_BULLET_HEIGHT		30
#define ANI_FIRE_BULLET			"ani-fire-bullet"
#define VELOCITY_X_FIRE_BULLET			0.018125f 
#define VELOCITY_Y_FIRE_BULLET			0.14f 
#define VELOCITY_Y_FIRE_BULLET_BOUNCE	0.425f 
#define INITIAL_DELTA_Y_FIRE_BULLET		0

#define DELTA_WIDTH_IN_LEFT_SIDE	80


class CFireBullet : public CGameObject
{
	
private:
	int owner = 0; // 0: Mario, 1: Venus
	Vector2 venusBulletDirection = Vector2(1,1);

public:
	CFireBullet(float x, float y, int nx, int = 0);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render(Vector2 finalPos);

	void PrepareForShooting();
	void UpdatePos(Vector2 pos, int nx=1);
	void SetVenusBulletDirection(Vector2 a) { venusBulletDirection = a;  }
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void OnHadCollided(LPGAMEOBJECT) override;
	virtual void CollidedLeftRight(LPGAMEOBJECT) override;
	virtual void CollidedTop(LPGAMEOBJECT) override;
};

