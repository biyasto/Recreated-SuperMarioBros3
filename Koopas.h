#pragma once
#include "Enemy.h"

#define KOOPAS_WALKING_SPEED		0.09f;

#define KOOPAS_BBOX_WIDTH			48
#define KOOPAS_BBOX_HEIGHT			77
#define KOOPAS_BBOX_CROUCH_WIDTH	38
#define KOOPAS_BBOX_CROUCH_HEIGHT	20
#define MAX_TIME_HELD				3000

#define KOOPAS_ANI_WILL_LIVE		"ani-red-koopa-troopa-will-live"
#define KOOPAS_ANI_WALKING			"ani-red-koopa-troopa-move"
#define KOOPAS_ANI_CROUCH			"ani-red-koopa-troopa-crouch"
#define KOOPAS_ANI_BEING_KICKED		"ani-red-koopa-troopa-shell-run"

struct SBeingHeldStatus {
	int step;
	int remainingTime;
};

class CKoopas : public CEnemy
{

	SBeingHeldStatus beingHeldStatus;

public:
	CKoopas(float x, float y);
	Vector2 GetBoundingBoxSize(EEnemyState) override;
	
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

	void CollidedTop(LPGAMEOBJECT) override;
	void CollidedBottom(LPGAMEOBJECT) override {} ;
	void Collided(LPGAMEOBJECT) override {};
	void OnHadCollided(LPGAMEOBJECT) override;
	//void BeingCollided(LPGAMEOBJECT) override;
	void BeingCollidedTopBottom(LPGAMEOBJECT) override;

	//void BeingCollidedTop(LPGAMEOBJECT) override;
	float GetDefaultWalkingSpeed() override { return KOOPAS_WALKING_SPEED; }
	
	bool ChangeState(EEnemyState newState, DWORD = 0) override;

	std::string GetAnimationIdFromState() override;


	
};