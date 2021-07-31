#include "BrownGoomba.h"
#include "TailAttackBox.h"
#include "PlayScene.h"
#include "MiniGoomba.h"

CBrownGoomba::CBrownGoomba(float x, float y) : CGoomba(x, y) {
	hasWingBehavior.time = 0;
	hasWingBehavior.step = 0;
}

std::string CBrownGoomba::GetAnimationIdFromState() {
	if (state.type == EEnemyState::WILL_DIE)
		return GOOMBA_ANI_WILL_DIE;
	else if (vy > 0) return BROWN_GOOMBA_FLY;
	else return BROWN_GOOMBA_IDLE;
		
}

Vector2 CBrownGoomba::GetBoundingBoxSize(EEnemyState st) {
	switch (st)
	{
	case EEnemyState::LIVE:
		return Vector2(GOOMBA_BBOX_WIDTH, GOOMBA_BBOX_HEIGHT);
	case EEnemyState::WILL_DIE:
		return Vector2(0, 0);
	case EEnemyState::DIE:
	case EEnemyState::ONESHOTDIE:
		return Vector2(0, 0);
	default:
		return Vector2(GOOMBA_BBOX_WIDTH, GOOMBA_BBOX_HEIGHT);
	}
}

void CBrownGoomba::ChangeStepHasWingBehavior(DWORD dt) {
	hasWingBehavior.time -= dt;
	if (hasWingBehavior.time <= 0) 
		{
			hasWingBehavior.step++;
			if (hasWingBehavior.step > GOOMBA_MAX_STEP)
			hasWingBehavior.step = 0;		
			hasWingBehavior.time = 1000;		
		}
}

void CBrownGoomba::ProcessHasWingBehavior() {
	switch (hasWingBehavior.step)
	{
	case 0:
		
		vx = -BROWN_GOOMBA_STEP;
		vy = -BROWN_GOOMBA_STEP;
		break;
	case 1:
		vx = -BROWN_GOOMBA_STEP;
		vy = BROWN_GOOMBA_STEP;
		break;
	case 2:
		
		vx = -BROWN_GOOMBA_STEP;
		vy = -BROWN_GOOMBA_STEP;
		break;
	case 3:
		vx = BROWN_GOOMBA_STEP;
		vy = BROWN_GOOMBA_STEP;
		break;
	case 4:
	
		vx = BROWN_GOOMBA_STEP;
		vy = -BROWN_GOOMBA_STEP;
		break;
	case 5:
		vx = BROWN_GOOMBA_STEP;
		vy = BROWN_GOOMBA_STEP;
		break;
	default: vx = 0; vy = 0;break;

	}
}

void CBrownGoomba::CollidedTop(LPGAMEOBJECT obj) {
	CGameObject::CollidedTop(obj);
	if (state.type == EEnemyState::LIVE)
		ProcessHasWingBehavior();
}

void CBrownGoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	if (state.type == EEnemyState::WILL_DIE) {
		ChangeState(EEnemyState::DIE); 
		return;
	}
	if (state.type == EEnemyState::DIE) return; 

	if (state.type == EEnemyState::LIVE) ChangeStepHasWingBehavior(dt);

	
	if (state.type != EEnemyState::ONESHOTDIE)
	{
		CEnemy::Update_Fly(dt, coObjects);
		ProcessHasWingBehavior();
	}
	else
	{
		CEnemy::Update(dt, coObjects);ApplyGravity();
	}
	UpdateWithCollision(coObjects);
	ShootBullet();
}


void CBrownGoomba::BeingCollidedTop(LPGAMEOBJECT obj) {
	CEnemy::BeingCollidedTop(obj);

	if (dynamic_cast<CMario*>(obj)) {
		if (state.type == EEnemyState::LIVE)
			ChangeState(EEnemyState::WILL_DIE, 1000);
	}

}
void CBrownGoomba::BeingCollided(LPGAMEOBJECT obj) {
	if (dynamic_cast<CTailAttackBox*>(obj)) {
		vy = -0.55f;
		//nx = (x - obj->x) > 0 ? 1 : -1;
		walkingSpeed = 0.1;

		ChangeState(EEnemyState::ONESHOTDIE, 1000);

	}
	else if (dynamic_cast<CFireBullet*>(obj)) {
		ChangeState(EEnemyState::ONESHOTDIE, 1000);
	}


}
void CBrownGoomba::ShootBullet() {
	vector<LPGAMEOBJECT> bullets = ((CPlayScene*)(CGame::GetInstance()->GetCurrentScene()))->GetEnemyBullets();
	for (int i = 0; i < bullets.size(); i++) {
		if (dynamic_cast<CMiniGoomba*>(bullets.at(i))) {
			Vector2 playerPos = ((CPlayScene*)(CGame::GetInstance()->GetCurrentScene()))->GetPlayer()->GetPosition();
			Vector2 bulletBeginPos = this->GetPosition();
			Vector2 direction;
	
			if (bullets.at(i)->isDisable) 
			{
				bullets.at(i)->vx = 0.48f;
				bullets.at(i)->vy = -0.23f;
				((CMiniGoomba*)(bullets.at(i)))->SetOwner(this);
				bullets.at(i)->SetPosition(bulletBeginPos);
				((CMiniGoomba*)bullets.at(i))->SetVenusBulletDirection(direction);
				bullets.at(i)->nx = nx;
				bullets.at(i)->isDisable = false;

			}
		}



	}
}
