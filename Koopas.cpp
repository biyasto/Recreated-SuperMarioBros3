#include "Koopas.h"
#include "TailAttackBox.h"
#include "Death.h"

CKoopas::CKoopas(float x, float y)
{
	this->x = x;
	this->y = y;
	this->nx = -1;
	useChangeDirectionAfterAxisCollide = true;
	ChangeState(EEnemyState::LIVE);
	beingHeldStatus.remainingTime = 0;
	beingHeldStatus.step = 0;
}


std::string CKoopas::GetAnimationIdFromState() {
	if (beingHeldStatus.step == 2) return KOOPAS_ANI_WILL_LIVE;
	switch (state.type)
	{
	case EEnemyState::LIVE:
		return KOOPAS_ANI_WALKING;
	case EEnemyState::WILL_LIVE:
		return KOOPAS_ANI_WILL_LIVE;
	case EEnemyState::WILL_DIE:
	case EEnemyState::BEING_HELD:
		return KOOPAS_ANI_CROUCH;
	case EEnemyState::BEING_KICKED:
		return KOOPAS_ANI_BEING_KICKED;
	default:
		return KOOPAS_ANI_WALKING;
	}
}

Vector2 CKoopas::GetBoundingBoxSize(EEnemyState st) {
	switch (st)
	{
	case EEnemyState::LIVE:
		return Vector2(KOOPAS_BBOX_WIDTH, KOOPAS_BBOX_HEIGHT);
	case EEnemyState::WILL_DIE:
	case EEnemyState::WILL_LIVE:
	case EEnemyState::BEING_KICKED:
		return Vector2(KOOPAS_BBOX_CROUCH_WIDTH, KOOPAS_BBOX_CROUCH_WIDTH);
	case EEnemyState::DIE:
	case EEnemyState::ONESHOTDIE:
		return Vector2(0, 0);
	case EEnemyState::BEING_HELD:
		return Vector2(KOOPAS_BBOX_WIDTH, KOOPAS_BBOX_WIDTH);
	default:
		return Vector2(KOOPAS_BBOX_WIDTH, KOOPAS_BBOX_HEIGHT);
	}
}

void CKoopas::OnHadCollided(LPGAMEOBJECT obj) {
	if(dynamic_cast<CMario*>(obj) && state.type != EEnemyState::WILL_DIE && state.type != EEnemyState::BEING_HELD)
		if(((CMario*)(obj))->GetAction() != MarioAction::EXPLODE)
			((CMario*)(obj))->BeingKilled();
}
void CKoopas::BeingCollidedTopBottom(LPGAMEOBJECT obj) {
	BeingCollided(obj);
	
	if (dynamic_cast<CMario*>(obj)) {
		if (state.type == EEnemyState::LIVE)
			ChangeState(EEnemyState::WILL_DIE, 5000);
		else
			BeingKicked(obj->GetPosition());
	}
}





void CKoopas::CollidedTop(LPGAMEOBJECT obj) {
	if (dy > 3.0f) {
		walkingScope.x = walkingScope.y = 0;
	}
	InitWtandingScope(obj);
}



void CKoopas::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state.type == EEnemyState::WILL_DIE) ChangeState(EEnemyState::WILL_LIVE,1500);

	else if (state.type == EEnemyState::WILL_LIVE) ChangeState(EEnemyState::LIVE, 0);

	else if(state.type == EEnemyState::BEING_KICKED) ChangeState(EEnemyState::DIE);

	else if (state.type == EEnemyState::BEING_HELD && beingHeldStatus.step != 0) {
		beingHeldStatus.remainingTime -= dt;
		if ((int)beingHeldStatus.remainingTime < 0) {
			if (beingHeldStatus.step == 1) {
				beingHeldStatus.remainingTime = MAX_TIME_HELD;
				beingHeldStatus.step = 2;
			}
			else if (beingHeldStatus.step == 2) {
				beingHeldStatus.step = 0;
				beingHeldStatus.remainingTime = 0;
				ChangeState(EEnemyState::LIVE);
				holdController->SetAction(MarioAction::IDLE);
			}
		}
		
	}

	DebugOut(ToWSTR(std::to_string((int)(beingHeldStatus.remainingTime)) + "\n").c_str());
	ApplyGravity();
	CEnemy::Update(dt, coObjects);
	ChangeDirectionAfterAxisCollide();
	UpdateWithCollision(coObjects);
	
}



bool CKoopas::ChangeState(EEnemyState newState, DWORD timeState) {
	if (CEnemy::ChangeState(newState, timeState)) {
		if (beingHeldStatus.step == 0 && (newState == EEnemyState::WILL_DIE)) {
			beingHeldStatus.remainingTime = MAX_TIME_HELD;
			beingHeldStatus.step = 1;
		}
		return true;
	}
	return false;
}