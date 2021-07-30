#pragma once
#include "Goomba.h"
#include "RedGoomba.h"
#define GOOMBA_HAS_WING_BBOX_WIDTH				48
#define GOOMBA_HAS_WING_BBOX_HEIGHT				65
#define GOOMBA_BBOX_WILL_DIE_HEIGHT				18


#define GOOMBA_BBOX_HEIGHT_DIE 9
#define GOOMBA_MAX_STEP 5

#define BROWN_GOOMBA_FLY				"ani-tan-para-goomba-flap" 
#define BROWN_GOOMBA_IDLE				"ani-tan-para-goomba-idle" 
#define BROWN_GOOMBA_STEP				0.05f; 


class CBrownGoomba : public CGoomba
{
	SHasWingBehavior hasWingBehavior;
public:
	CBrownGoomba(float x, float y);
	virtual Vector2 GetBoundingBoxSize(EEnemyState) override;

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;
	
	void CollidedTop(LPGAMEOBJECT) override;
	void BeingCollidedTop(LPGAMEOBJECT) override;
	void BeingCollided(LPGAMEOBJECT obj) override;

	void ChangeStepHasWingBehavior(DWORD);
	virtual void ProcessHasWingBehavior();
	void ShootBullet();

	virtual std::string GetAnimationIdFromState() override;
};
