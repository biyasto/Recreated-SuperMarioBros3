#pragma once
#include "Enemy.h"


class CBoomerangBrother : public CEnemy
{
	int renderNX = 1;
public:
	CBoomerangBrother(float, float);
	
	virtual Vector2 GetBoundingBoxSize(EEnemyState) override;
	
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;
	virtual void Render(Vector2) override;

	void BeingCollidedTop(LPGAMEOBJECT) override;

	void CollidedTop(LPGAMEOBJECT obj);

	bool ShootBullet();
	
	float GetDefaultWalkingSpeed() override { return 0.09f; }

	virtual std::string GetAnimationIdFromState() override;
	
};

