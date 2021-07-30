#pragma once
#include "RedGoomba.h"

#define KOOPASFLY_WALKING_SPEED		0.09f;

#define KOOPASFLY_BBOX_WIDTH			48
#define KOOPASFLY_BBOX_HEIGHT			77

#define KOOPASFLY_ANI_WALKING "ani-green-koopa-paratroopa-fly"

class CKoopasFly : public CRedGoomba
{
	

public:
	CKoopasFly(float x, float y);
	Vector2 GetBoundingBoxSize(EEnemyState) override;
	virtual std::string GetAnimationIdFromState() override;
	void ProcessHasWingBehavior() override;


};