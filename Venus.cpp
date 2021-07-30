#include "Venus.h"
#include "PlayScene.h"
#include "TailAttackBox.h"



CVenus::CVenus(Vector2 initPos, Vector2 size, int type) : CEnemy() {
	this->initPos = initPos;
	this->size = size;
	this->type = type;
	this->x = initPos.x;
	this->y = initPos.y;
	ChangeState(EVenusState::HEADDOWN_IDLE);
	y = initPos.y + size.y;
}

bool CVenus::ShootBullet() {
	vector<LPGAMEOBJECT> bullets = ((CPlayScene*)(CGame::GetInstance()->GetCurrentScene()))->GetEnemyBullets();
	for (int i = 0; i < bullets.size(); i++) {
		if(dynamic_cast<CFireBullet*>(bullets.at(i))){
			Vector2 playerPos = ((CPlayScene*)(CGame::GetInstance()->GetCurrentScene()))->GetPlayer()->GetPosition();
			Vector2 bulletBeginPos = Vector2(this->x + 25 * nx, this->y - 40);
			Vector2 direction;
			
			direction.x = (playerPos.x - x) > 0 ? 1 : -1;
			direction.y = (playerPos.y - y) > 0 ? 1 : -1;
			if (bullets.at(i)->isDisable) {
				bullets.at(i)->vy = VELOCITY_Y_FIRE_BULLET * 0.3;
				bullets.at(i)->SetPosition(bulletBeginPos);
				((CFireBullet*)bullets.at(i))->SetVenusBulletDirection(direction);
				bullets.at(i)->nx = nx;
				bullets.at(i)->isDisable = false;
				return true;
			}
		}
		
	}
	return false;
}

void CVenus::OnHadCollided(LPGAMEOBJECT obj) {
	if (dynamic_cast<CTailAttackBox*>(obj)) {
		vy = -0.55f;
		nx = (x - obj->x) > 0 ? 1 : -1;
		walkingSpeed = 0.1;
		ChangeState(EEnemyState::WILL_DIE);

	}
	if (dynamic_cast<CFireBullet*>(obj)) {
		vy = -0.55f;
		nx = (x - obj->x) > 0 ? 1 : -1;
		walkingSpeed = 0.1;
		ChangeState(EEnemyState::WILL_DIE);

	}
}

void CVenus::BeingCollidedTop(LPGAMEOBJECT obj) {
	CEnemy::BeingCollidedLeftRight(obj);
	if (dynamic_cast<CMario*>(obj)) {
		((CMario*)(obj))->BeingBouncedAfterJumpInTopEnemy();
	}
	if (dynamic_cast<CFireBullet*>(obj)) {
		vy = -0.55f;
		nx = (x - obj->x) > 0 ? 1 : -1;
		walkingSpeed = 0.1;
		ChangeState(EEnemyState::WILL_DIE);

	}
}

void CVenus::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	if (CEnemy::state.type == EEnemyState::DIE) return;
	int playerX = ((CPlayScene*)(CGame::GetInstance()->GetCurrentScene()))->GetPlayer()->GetPosition().x;
	if (playerX > this->x) nx = 1; else nx = -1;
	
	int a = 0;
	if (state.type == EVenusState::HEADINGUP) {
		y = initPos.y + ((float)(state.timeRemaining *1.0f / VENUS_TIME_DURATION_HEADING_DOWNUP)) * size.y;
		ChangeState(EVenusState::HEADUP_IDLE);
	}
	else if (state.type == EVenusState::HEADINGDOWN) {
		y = initPos.y + (1-(float)(state.timeRemaining * 1.0f / VENUS_TIME_DURATION_HEADING_DOWNUP)) * size.y;
		ChangeState(EVenusState::HEADDOWN_IDLE);
	}
	else if (state.type == EVenusState::HEADUP_IDLE) {
		y = initPos.y;
		ChangeState(EVenusState::HEADINGDOWN);
	}
	else if (state.type == EVenusState::HEADDOWN_IDLE) {
		y = initPos.y + size.y;
		ChangeState(EVenusState::HEADINGUP);
	}

	state.timeRemaining -= dt;
	
}




void CVenus::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	if (CEnemy::state.type == EEnemyState::DIE) {
		left = top = right = bottom = 0;
		return;
	}
	left = x - size.x / 2;
	top = y - size.y / 2;
	right = x + size.x / 2;
	bottom = y + size.y / 2;
}

std::string CVenus::GetAnimationIdFromState() {
	if(type == 0)
		switch (state.type) {
		case EVenusState::HEADINGUP:
			return "ani-red-venus-fire-trap-headup";
		case EVenusState::HEADUP_IDLE:
			return "ani-red-venus-fire-trap-headup-idle";
		case EVenusState::HEADINGDOWN:
			return "ani-red-venus-fire-trap-headdown";
		case EVenusState::HEADDOWN_IDLE:
			return "ani-red-venus-fire-trap-headdown-idle";

		default:
			return "ani-red-venus-fire-trap-headup";
		}
	else if (type == 1)
		switch (state.type) {
		case EVenusState::HEADINGUP:
			return "ani-green-piranha-plant-attack";
		case EVenusState::HEADUP_IDLE:
			return "ani-green-piranha-plant-attack";
		case EVenusState::HEADINGDOWN:
			return "ani-green-piranha-plant-attack";
		case EVenusState::HEADDOWN_IDLE:
			return "ani-green-piranha-plant-attack";

		default:
			return "ani-red-venus-fire-trap-headup";
		}
	else if (type == 2)
		switch (state.type) {
		case EVenusState::HEADINGUP:
			return "ani-green-venus-fire-trap-headup";
		case EVenusState::HEADUP_IDLE:
			return "ani-green-venus-fire-trap-headup-idle";
		case EVenusState::HEADINGDOWN:
			return "ani-green-venus-fire-trap-headdown";
		case EVenusState::HEADDOWN_IDLE:
			return "ani-green-venus-fire-trap-headdown-idle";

		default:
			return "ani-green-venus-fire-trap-headup";
		}
}


bool CVenus::ChangeState(EEnemyState newState, DWORD timeState) {
	if (newState == EEnemyState::ONESHOTDIE) {
		ChangeState(EEnemyState::DIE);
		
	}
	else CEnemy::ChangeState(newState, timeState);
	return true;
}

void CVenus::ChangeState(EVenusState newState) {
	if (newState == EVenusState::HEADINGUP) {
		int playerX = ((CPlayScene*)(CGame::GetInstance()->GetCurrentScene()))->GetPlayer()->GetPosition().x;
		if(abs(x - playerX) < 160) return;
	}
	if (state.timeRemaining <= 0) {
		this->state.type = newState;
		if(newState == EVenusState::HEADINGDOWN || newState == EVenusState::HEADINGUP)
			this->state.timeRemaining = VENUS_TIME_DURATION_HEADING_DOWNUP;
		else {
			int headDownTime = 3000;
			if (newState == EVenusState::HEADUP_IDLE && (type == 0 || type == 2)) {
				ShootBullet();
				headDownTime = 0;
			}
				
			this->state.timeRemaining = VENUS_TIME_DURATION_IDLE + headDownTime;
		}
			
	}
}


void CVenus::Render(Vector2 finalPos) {
	if (CEnemy::state.type == EEnemyState::DIE) return;
	CEnemy::Render(finalPos);
}