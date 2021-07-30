#include "RedGoomba.h"

CRedGoomba::CRedGoomba(float x, float y) : CGoomba(x, y) {
	hasWingBehavior.time = 0;
	hasWingBehavior.step = 0;
}

std::string CRedGoomba::GetAnimationIdFromState() {
	switch (state.type)
	{
	case EEnemyState::LIVE: {
		if (hasWingBehavior.step == 0) return REDGOOMBA_CLOSE_WING_WALKING;
		else return REDGOOMBA_OPEN_WING_WALKING;
	}

	case EEnemyState::LIVE1:
		return REDGOOMBA_NO_WING_WALKING;
	
	
	case EEnemyState::ONESHOTDIE:
		return REDGOOMBA_NO_WING_WALKING;
	case EEnemyState::WILL_DIE:
		return REDGOOMBA_WILLDIE;
	default:
		return REDGOOMBA_NO_WING_WALKING;
	}
}

Vector2 CRedGoomba::GetBoundingBoxSize(EEnemyState st) {
	switch (st)
	{
	case EEnemyState::LIVE:
		if (hasWingBehavior.step == 0) return Vector2(GOOMBA_BBOX_WIDTH, GOOMBA_BBOX_HEIGHT);
		else return Vector2(GOOMBA_HAS_WING_BBOX_WIDTH, GOOMBA_HAS_WING_BBOX_HEIGHT);
		
	case EEnemyState::WILL_DIE:
		return Vector2(0, 0);
	case EEnemyState::DIE:
	case EEnemyState::ONESHOTDIE:
		return Vector2(0, 0);
	default:
		return Vector2(GOOMBA_BBOX_WIDTH, GOOMBA_BBOX_HEIGHT);
	}
}

void CRedGoomba::ChangeStepHasWingBehavior(DWORD dt) {
	hasWingBehavior.time -= dt;
	if (hasWingBehavior.time <= 0) {
		switch (hasWingBehavior.step)
		{
		case 0:
			y -= GOOMBA_HAS_WING_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT;
			hasWingBehavior.step = 1;
			hasWingBehavior.time = 1000;
			break;
		case 1:
			hasWingBehavior.step = 2;
			hasWingBehavior.time = 500;
			break;
		case 2:
			hasWingBehavior.step = 0;
			hasWingBehavior.time = 1000;
			break;

		}
	}
}

void CRedGoomba::ProcessHasWingBehavior() {
	switch (hasWingBehavior.step)
	{
	case 0:
		break;
	case 1:
		vy = -0.3f;
		break;
	case 2:
		vy = -0.55f;
		break;

	}
}

void CRedGoomba::CollidedTop(LPGAMEOBJECT obj) {
	CGameObject::CollidedTop(obj);
	if(state.type == EEnemyState::LIVE)
		ProcessHasWingBehavior();
}

void CRedGoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	if (state.type == EEnemyState::WILL_DIE) {
		ChangeState(EEnemyState::DIE);
		return;
	}
	if (state.type == EEnemyState::DIE) return;

	if (state.type == EEnemyState::LIVE) ChangeStepHasWingBehavior(dt);

	ApplyGravity();
	CEnemy::Update(dt, coObjects);

	ChangeDirectionAfterAxisCollide();
	UpdateWithCollision(coObjects);

}


void CRedGoomba::BeingCollidedTop(LPGAMEOBJECT obj) {
	CEnemy::BeingCollidedTop(obj);

	if (dynamic_cast<CMario*>(obj)) {
		if (state.type == EEnemyState::LIVE)
			ChangeState(EEnemyState::LIVE1);
		else if(state.type == EEnemyState::LIVE1)
			ChangeState(EEnemyState::WILL_DIE, 1000);
	}

}