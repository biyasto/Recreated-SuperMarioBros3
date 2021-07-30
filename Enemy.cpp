#include "Enemy.h"
#include "PlayScene.h"
#include "AddingPointEffect.h"
#include "TailAttackBox.h"
#include "Death.h"

CEnemy::CEnemy() {
	walkingSpeed = 0;
	walkingScope = Vector2(0, 0);
}

CEnemy::CEnemy(float x, float y, int nx) {
	walkingSpeed = 0;
	walkingScope = Vector2(0, 0);
	this->x = x;
	this->y = y;
	this->nx = nx;
}

void CEnemy::ChangeDirection() {
	dx *= -1;
	nx *= -1;
}

void CEnemy::InitWtandingScope(LPGAMEOBJECT obj) {
	if (walkingScope.x == 0 && walkingScope.y == 0) {
		float left, top, right, bottom;
		obj->GetBoundingBox(left, top, right, bottom);
		walkingScope = Vector2(left, right);
		return;
	}
}

void CEnemy::ChangeDirectionAfterAxisCollide() {
	if (!useChangeDirectionAfterAxisCollide) return;
	float left, top, right, bottom;
	GetBoundingBox(left, top, right, bottom);
	if ((x + dx < walkingScope.x || x + dx > walkingScope.y ) && (walkingScope.x != 0 || walkingScope.y != 0)) {
		ChangeDirection();
	}
}

void CEnemy::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (GetBoundingBoxSize().x == 0 && GetBoundingBoxSize().y == 0) {
		left = top = right = bottom = 0;
	}
	else {
		left = x - GetBoundingBoxSize().x / 2;
		top = y - GetBoundingBoxSize().y / 2;
		right = x + GetBoundingBoxSize().x / 2;
		bottom = y + GetBoundingBoxSize().y / 2;
	}

}


void CEnemy::SetState(EEnemyState newState, DWORD timeState) {
	Vector2 currentSize = GetBoundingBoxSize();
	Vector2 newSize = GetBoundingBoxSize(newState);
	state.type = newState;
	state.timeState = timeState;
	state.timeBegin = GetTickCount64();
	if (newSize.x == 0 && newSize.y == 0) return;
	this->y -= (newSize.y - currentSize.y) / 2;
	this->x -= (newSize.x - currentSize.x) / 2;
}

void CEnemy::BeingHeldProcess() {
	if (state.type != EEnemyState::BEING_HELD) return;
	if (holdController->GetAction() != MarioAction::HOLD) {
		holdController->SetAction(MarioAction::KICK, 200);
		BeingKicked(holdController->GetPosition());
		return;
	}
	vx = vy = 0;
	//SetPosition(holdController->GetPosition() + Vector2(HOLDING_DISTANCE * (holdController->GetNX()), 0));
}

void CEnemy::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	vx = walkingSpeed * nx;
	SetEffect(EExtraEffect::NONE);
	BeingHeldProcess();
	CGameObject::Update(dt, coObjects);
}
void CEnemy::Update_Fly(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {

	SetEffect(EExtraEffect::NONE);
	BeingHeldProcess();
	CGameObject::Update(dt, coObjects);
}


void CEnemy::Render(Vector2 finalPos) {
	if (state.type == EEnemyState::DIE) return;
	//RenderBoundingBox(finalPos);
	CAnimations::GetInstance()->Get(GetAnimationIdFromState())->Render(finalPos, Vector2(-nx, ny), 255);
	RenderExtraEffect(finalPos);

}


void CEnemy::OnHadCollided(LPGAMEOBJECT obj) {
	this->BeingCollided(obj);
};

void CEnemy::BeingCollidedTop(LPGAMEOBJECT obj) {
	if (dynamic_cast<CMario*>(obj)) {
		((CMario*)(obj))->BeingBouncedAfterJumpInTopEnemy();
	}
	this->BeingCollidedTopBottom(obj);
};

void CEnemy::CollidedLeftRight(LPGAMEOBJECT obj) {
	CGameObject::CollidedLeftRight(obj);
	if (dynamic_cast<CEnemy*>(obj)) {
		if (GetState() == EEnemyState::BEING_KICKED)
			return;
	}
	ChangeDirection();
}

void CEnemy::BeingCollidedLeftRight(LPGAMEOBJECT obj) {
	
	if (dynamic_cast<CMario*>(obj)) {
		if (((CMario*)(obj))->GetAction() == MarioAction::ATTACK) return;
		switch (state.type)
		{
		case EEnemyState::WILL_DIE:
			if (((CMario*)(obj))->isHoldingKey(DIK_A)) {
				holdController = ((CMario*)(obj));
				((CMario*)(obj))->SetAction(MarioAction::HOLD);
				((CMario*)(obj))->SetHoldingObj(this);
				SetState(EEnemyState::BEING_HELD);

			}
			else {
				((CMario*)(obj))->SetAction(MarioAction::KICK, 200);
				BeingKicked(obj->GetPosition());
			}

			break;
		case EEnemyState::LIVE:
			KillMario((CMario*)obj);
			break;
		case EEnemyState::BEING_KICKED:
			if (((CMario*)(obj))->GetAction() != MarioAction::KICK) {
				KillMario((CMario*)obj);
			}
			break;

		case EEnemyState::DIE:
			break;
		}
	}
	BeingCollided(obj);
}


void CEnemy::BeingKicked(Vector2 pos) {
	
	float left, top, right, bottom;
	this->GetBoundingBox(left, top, right, bottom);
	if (pos.x < (left + right) / 2)
		nx = 1;
	else
		nx = -1;
	walkingSpeed = 0.54;
	useChangeDirectionAfterAxisCollide = false;
	isTemp = true;
	((CPlayScene*)(CGame::GetInstance()->GetCurrentScene()))->PushHighPriorityObjects(this);
	SetState(EEnemyState::BEING_KICKED, 10000);
}

void CEnemy::BeingCollided(LPGAMEOBJECT obj) {
	if (dynamic_cast<CTailAttackBox*>(obj)) {
		vy = -0.55f;
		nx = (x - obj->x) > 0 ? 1 : -1;
		walkingSpeed = 0.1;

		SwitchEffect(EExtraEffect::BEING_DAMAGED);
		ChangeState(EEnemyState::ONESHOTDIE);
	
	}
	else if (dynamic_cast<CEnemy*>(obj)) {
		if (((CEnemy*)obj)->GetState() == EEnemyState::BEING_KICKED) {
			vy = -0.55f;
			nx = (x - obj->x) > 0 ? 1 : -1;
			walkingSpeed = 0.1;

			SwitchEffect(EExtraEffect::BEING_DAMAGED);
			ChangeState(EEnemyState::ONESHOTDIE);
		}
		
	}

	else if (dynamic_cast<CEnemy*>(obj)) {
		if (this->GetState() == EEnemyState::BEING_KICKED) {
			vy = -0.55f;
			nx = (x - obj->x) > 0 ? 1 : -1;
			walkingSpeed = 0.1;

			((CEnemy*)obj)->SwitchEffect(EExtraEffect::BEING_DAMAGED);
			((CEnemy*)obj)->ChangeState(EEnemyState::ONESHOTDIE);
		}

	}
	else if (dynamic_cast<CFireBullet*>(obj)) {
		ChangeState(EEnemyState::ONESHOTDIE);
	}
	else if (dynamic_cast<CDeath*>(obj)) {
		if (((CEnemy*)obj)->GetState() == EEnemyState::BEING_KICKED) {
			vy = -0.55f;
			nx = (x - obj->x) > 0 ? 1 : -1;
			walkingSpeed = 0.1;

			SwitchEffect(EExtraEffect::BEING_DAMAGED);
			ChangeState(EEnemyState::ONESHOTDIE);
		}

	}

	
}



void CEnemy::KillMario(CMario* mario) {
	mario->BeingKilled();
}



bool CEnemy::ChangeState(EEnemyState newState, DWORD newTimeState)
{
	
	if (newState != EEnemyState::ONESHOTDIE) {
		if (GetTickCount64() < state.timeBegin + state.timeState) return false;
	}
	switch (newState)
	{
	case EEnemyState::DIE:
		if (state.type == EEnemyState::WILL_DIE || state.type == EEnemyState::BEING_KICKED)
			SetState(newState, newTimeState);
		break;
	case EEnemyState::WILL_DIE:
		if (state.type == EEnemyState::LIVE || state.type == EEnemyState::LIVE1) {
			((CPlayScene*)(CGame::GetInstance()->GetCurrentScene()))->PushEffects(new CAdding100PointEffect(GetPosition(), Vector2(0, -0.11)));
			walkingSpeed = 0;
			SetState(newState, newTimeState);
		}
		break;
	case EEnemyState::BEING_KICKED:
		if (state.type == EEnemyState::WILL_DIE || state.type == EEnemyState::BEING_HELD) {
			((CPlayScene*)(CGame::GetInstance()->GetCurrentScene()))->PushEffects(new CAdding100PointEffect(GetPosition(), Vector2(0, -0.11)));
			SetState(newState, 3000);
		}
		break;
	
	case EEnemyState::LIVE:
		isTemp = false;
		walkingSpeed = GetDefaultWalkingSpeed();
		SetState(newState, newTimeState);
		break;
	case EEnemyState::LIVE1:
		isTemp = false;
		if (state.type == EEnemyState::LIVE) {
			walkingSpeed = GetDefaultWalkingSpeed();
			SetState(newState, newTimeState);
		}
		
		break;
	case EEnemyState::ONESHOTDIE:
		vy = -0.7f;
		ny = -1;
		((CPlayScene*)(CGame::GetInstance()->GetCurrentScene()))->PushEffects(new CAdding100PointEffect(GetPosition(), Vector2(0, -0.11)));
		SetState(newState, newTimeState);
		break;


	default:
		SetState(newState, newTimeState);
		break;
	}

	return true;
}
