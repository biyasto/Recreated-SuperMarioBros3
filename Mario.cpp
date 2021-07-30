#include <algorithm>
#include <assert.h>
#include "Utils.h"
#include "Mario.h"
#include "Enemy.h"
#include "FireBullet.h"
#include "Boomerang.h"
#include "MiniGoomba.h"
#include "PlayScene.h"
#include "Mushroom.h"
#include "CAdding1UpEffect.h"


#define HOLDING_DISTANCE			40

#pragma region Init
CMario::CMario(float x, float y) : CGameObject()
{

	start_x = x;
	start_y = y;

	this->x = x;
	this->y = y;
	allowOthersGoThrough = true;
}
#pragma endregion

#pragma region Update, Render

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{


	if (state.action == MarioAction::DIE && IsReadyToChangeAction() || finishStep == 3) {
		((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->SwitchToSelectionScene();
	}
	if (state.action == MarioAction::GETTING_INTO_THE_PIPE) {

		if (getIntoThePipe.time <= 0) {
			// Finish Get Into Hole
			SetAction(MarioAction::IDLE);
			deltaRender.x = deltaRender.y = 0;
			if (getIntoThePipe.OnFinish != nullptr) getIntoThePipe.OnFinish();
		}

		DWORD time = getIntoThePipe.time - dt > 0 ? dt : getIntoThePipe.time;
		getIntoThePipe.time -= time;
		deltaRender.x += getIntoThePipe.distance.x * (time * 1.0f / getIntoThePipe.totalTime * 1.0f);
		deltaRender.y += getIntoThePipe.distance.y * (time * 1.0f / getIntoThePipe.totalTime * 1.0f);
		return;
	}

	CGameObject::Update(dt);
	UpdateWithCollision(coObjects);

	if (finishStep == 1) {
		ChangeAction(MarioAction::WALK);
	}
	else if (finishStep == 2) {
		if (IsReadyToChangeAction()) finishStep = 3;
	}

	// Increase velocity if in limit
	float vxmax = holdingKeys[DIK_A] ? VELOCITY_X_MAX_RUN : VELOCITY_X_MAX_WALK;
	ax = ax * (1 + (holdingKeys[DIK_A] ? ACCELERATION_X_RUN_GREATER_RATIO : 0));

	if (state.action != MarioAction::DIE)
		if (abs(vx) < vxmax)
			vx += ax * dt;

	if (finishStep == 0)
		ApplyFriction();
	if (state.action == MarioAction::FALL_SLIGHTLY)
		vy = ACCELERATION_Y_GRAVITY * dt * 1.7;

	else if (state.action != MarioAction::FLY)
		ApplyGravity();
	else
		ChangeAction(MarioAction::FALL);





	if (vx != 0) {
		if (abs(vx) < VELOCITY_X_MIN_FOR_RUN && state.action != MarioAction::CROUCH) {
			ChangeAction(MarioAction::WALK);
		}
		else {

			if (powerX < POWER_X_MIN_FOR_SPEEDUP) {
				powerX += abs(dx) * POWER_X_RATIO_GAIN;
				ChangeAction(MarioAction::RUN);
			}
			else {
				powerX = POWER_X_WHEN_GETTING_SPEEDUP;
				ChangeAction(MarioAction::SPEEDUP);
			}

		}

	}
	if (state.action == MarioAction::HOLD) {
		holdingObj->SetPosition(GetPosition() + Vector2(HOLDING_DISTANCE * GetNX(), 0));
	}

	// SKID
	if (vx * nx < 0 && abs(vx) > VELOCITY_X_MIN_FOR_SKID) {
		ChangeAction(MarioAction::SKID, 250);

	}

	if (powerX > 0) powerX -= POWER_X_LOSE_ALWAYS;


	if (untouchable.isUntouchable) {
		untouchable.remainingTime -= dt;
		if (untouchable.remainingTime <= 0) untouchable.isUntouchable = false;
	}

	ResetTempValues(); 
}

void CMario::Render(Vector2 finalPos) {


	std::vector<MarioAction> animationFlipY = { MarioAction::SKID };

	ChangeRenderAnimation(GetAnimationIdFromState());

	for (int i = 0; i < animationFlipY.size(); i++) {
		if (state.action == animationFlipY[i]) {
			SRenderAnimation ani;
			ani.AnimationID = GetAnimationIdFromState();
			ani.isFlipY = true;
			ChangeRenderAnimation(ani);
		}
	}


	float l, t, r, b;

	GetBoundingBox(l, t, r, b);

	//RenderBoundingBox(Vector2(finalPos.x + (l-this->x), finalPos.y + (t-this->y)));
	//RenderBoundingBox(finalPos);

	if (untouchable.isUntouchable && GetTickCount64() % 100 > 50) return;


	CAnimations::GetInstance()->Get(renderAnimation.AnimationID)->Render(finalPos + deltaRender, Vector2(nx * (renderAnimation.isFlipY ? -1 : 1), ny), 255);

};

#pragma endregion

#pragma region Collided

void CMario::Collided(LPGAMEOBJECT obj) {
	CGameObject::Collided(obj);
	if (dynamic_cast<Mushroom*>(obj)) {
		obj->x = 0; obj->y = 0;
		obj->isDisable = true;
		y -= 15;
		if(((Mushroom*)(obj))->isPowerUp()==true)
		{
				ChangeAction(MarioAction::UPGRADE_LV);
		}
		if (((Mushroom*)(obj))->isPowerUp() == false)
		{
			((CPlayScene*)(CGame::GetInstance()->GetCurrentScene()))->PushEffects(new CAdding1UpEffect(Vector2(x, y), Vector2(0, -0.13)));
		}
	}
}

void CMario::BeingCollided(LPGAMEOBJECT obj) {
	if (dynamic_cast<CFireBullet*>(obj) || dynamic_cast<CBoomerang*>(obj) || dynamic_cast<CMiniGoomba*>(obj)) {
		BeingKilled();
	}
}

void CMario::NoCollided() {
	if (vy > 0 && (dy > 5 || state.action == MarioAction::FALL_SLIGHTLY)) ChangeAction(MarioAction::FALL);
}

void CMario::CollidedLeft(LPGAMEOBJECT obj) {
	if (finishStep == 1) {
		state.beginAction = GetTickCount64();
		state.timeAction = 500;
		finishStep = 2;
	}
	CGameObject::CollidedLeft(obj);
}

void CMario::CollidedBottom(LPGAMEOBJECT obj) {
	if (state.action == MarioAction::DIE) return;
	CGameObject::CollidedBottom(obj);
}

void CMario::CollidedTop(LPGAMEOBJECT obj) {

	ChangeBasePosition(GetPosition());
	if (state.action == MarioAction::DIE) return;
	if (state.action == MarioAction::FALL_SLIGHTLY) state.timeAction = 0;
	if (state.action != MarioAction::CROUCH) {
		canJump = true;
		ChangeAction(MarioAction::IDLE);
		if (powerX > 0 && abs(vx) < VELOCITY_X_MIN_FOR_RUN)
			powerX -= POWER_X_LOSE_IN_GROUND;
	}

	CGameObject::CollidedTop(obj);
	if (finishStep == 1) {
		vx = 0.24f;
		nx = 1;
	}

}


#pragma endregion

#pragma region BoundingBox

Vector2 CMario::GetBoundingBoxSize() {
	return GetBoundingBoxSize(type, state.action);
}

Vector2 CMario::GetBoundingBoxSize(MarioType mType, MarioAction mAction) {
	if (mAction == MarioAction::DIE) return Vector2(0, 0);
	if (mAction == MarioAction::CROUCH) return Vector2(MARIO_SMALL_BBOX_WIDTH, MARIO_SMALL_BBOX_HEIGHT);
	switch (mType)
	{
	case MarioType::SMALL:
		return Vector2(MARIO_SMALL_BBOX_WIDTH, MARIO_SMALL_BBOX_HEIGHT);

	case MarioType::TALL:
	case MarioType::FIRE:
	case MarioType::TAIL:
		return Vector2(MARIO_BIG_BBOX_WIDTH, MARIO_BIG_BBOX_HEIGHT);



	default:
		return Vector2(MARIO_SMALL_BBOX_WIDTH, MARIO_SMALL_BBOX_HEIGHT);
		break;
	}
}

void CMario::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	if (state.action == MarioAction::DIE) { left = top = right = bottom = 0; return; }
	left = x - GetBoundingBoxSize(type, state.action).x / 2;
	top = y - GetBoundingBoxSize(type, state.action).y / 2;
	right = x + GetBoundingBoxSize(type, state.action).x / 2;
	bottom = y + GetBoundingBoxSize(type, state.action).y / 2;
	/*if (left == right || bottom == top) {
		int a = 9;
	}*/
}

#pragma endregion

#pragma region Input

void CMario::ProcessKeyboard(SKeyboardEvent kEvent)
{
	if (state.action == MarioAction::EXPLODE || state.action == MarioAction::UPGRADE_LV || finishStep > 0) return;

	if (kEvent.isKeyUp == true) {
		holdingKeys[kEvent.key] = false;
		return;
	}

	if (kEvent.isHold == false) holdingKeys[kEvent.key] = true;

	switch (kEvent.key)
	{
		//trai
	case DIK_LEFT:
		ax = -ACCELERATION_X_WALK;
		nx = -1;
		break;
		//phai
	case DIK_RIGHT:
		ax = ACCELERATION_X_WALK;
		nx = 1;
		break;
		//xuong pipe
	case DIK_DOWN:
		if (kEvent.isHold) ChangeAction(MarioAction::CROUCH);
		break;

	case DIK_S:
		//Nhay
		if (!kEvent.isHold) {
			if (!kEvent.isKeyUp) ChangeAction(MarioAction::JUMP);
		}
		else ChangeAction(MarioAction::HIGH_JUMP);

		break;
	case DIK_X:
		//Nhay
		if (!kEvent.isHold) {
			if (!kEvent.isKeyUp) ChangeAction(MarioAction::JUMP);
		}

	default:
		if (this->state.action == MarioAction::CROUCH)
			ChangeAction(MarioAction::IDLE);
		break;
	}

}

#pragma endregion

#pragma region Animation

std::string CMario::GetAnimationIdFromState() {
	std::string typeId;
	std::string actionId;
	switch (type)
	{
	case MarioType::SMALL:
		typeId = "ani-small-mario";
		break;
	case MarioType::TALL:
		typeId = "ani-big-mario";
		break;
	case MarioType::TAIL:
		typeId = "ani-raccoon-mario";
		break;
	case MarioType::FIRE:
		typeId = "ani-fire-mario";
		break;

	default:
		typeId = "ani-small-mario";
		break;
	}

	switch (state.action)
	{
	case MarioAction::IDLE:
		actionId = "idle";
		break;
	case MarioAction::WALK:
		actionId = "walk";
		break;
	case MarioAction::RUN:
		actionId = "run";
		break;
	case MarioAction::SPEEDUP:
		actionId = "speed-up";
		break;
	case MarioAction::JUMP:
		actionId = "jump";
		break;
	case MarioAction::HIGH_JUMP:
		actionId = "jump";
		break;
	case MarioAction::CROUCH:
		actionId = "crouch";
		break;
	case MarioAction::FLY:
		actionId = "fly";
		break;
	case MarioAction::FALL_SLIGHTLY:
		actionId = "fall-slightly";
		break;
	case MarioAction::FALL:
		actionId = "fall";
		break;
	case MarioAction::SKID:
		actionId = "skid";
		break;
	case MarioAction::ATTACK:
		actionId = "attack";
		break;
	case MarioAction::KICK:
		actionId = "kick";
		break;
	case MarioAction::HOLD:
		actionId = "hold";
		break;
	case MarioAction::EXPLODE:
	case MarioAction::UPGRADE_LV:
		typeId = "ani-mario";
		actionId = "damaged";
		break;
	case MarioAction::DIE:
		actionId = "die";
		break;
	case MarioAction::GETTING_INTO_THE_PIPE:
		actionId = "idle-front";
		break;
	default:
		actionId = "idle";
		break;
	}

	return typeId + "-" + actionId;
}

#pragma endregion

#pragma region Action

void CMario::BeingBouncedAfterJumpInTopEnemy() {
	vy = -VELOCITY_Y_AFTER_COLLIDE_TOP_ENEMY;
}

void CMario::BeingBouncedAfterJumpInNoteBlock() {
	vy = -VELOCITY_Y_BEING_BOUNCED_NOTEBLOCK;
}

void CMario::SecretBeingBouncedAfterJumpInNoteBlock() {
	vy = -VELOCITY_Y_SECRET_BEING_BOUNCED_NOTEBLOCK;
}
void CMario::Teleport(float tele_x,float tele_y,float vel_x,float vel_y)
{
	SetPosition(tele_x, tele_y);
	SetSpeed(0, 0);
}

void CMario::BeingKilled() {

	if (state.action == MarioAction::DIE) return;

	if (type == MarioType::SMALL && untouchable.isUntouchable == false) {
		vy = -0.7f;
		vx = ax = ay = 0;
		SetAction(MarioAction::DIE, 1500);
		return;
	}

	if (untouchable.isUntouchable == false)
		ChangeAction(MarioAction::EXPLODE);
}

void CMario::BeginUntouchable() {
	untouchable.isUntouchable = true;
	untouchable.remainingTime = UNTOUCHABLE_TIME;
}

void CMario::GetIntoThePipe(Vector2 distance, CallbackTypeScene callback) {
	getIntoThePipe.distance = distance;
	getIntoThePipe.OnFinish = callback;
	getIntoThePipe.time = getIntoThePipe.totalTime;
	SetAction(MarioAction::GETTING_INTO_THE_PIPE);
}

void CMario::SetAction(MarioAction newAction, DWORD timeAction) {

	if (!IsReadyToChangeAction()) return;

	Vector2 oldBBox = GetBoundingBoxSize(type, state.action);
	Vector2 newBBox = GetBoundingBoxSize(type, newAction);
	x -= (newBBox.x - oldBBox.x) / 2;
	y -= (newBBox.y - oldBBox.y) / 2;

	state.action = newAction;
	state.beginAction = GetTickCount64();
	state.timeAction = timeAction;

}

bool CMario::ChangeAction(MarioAction newAction, DWORD timeAction) {



	if (state.action == MarioAction::DIE || state.action == MarioAction::GETTING_INTO_THE_PIPE) return false;

	if (state.action == MarioAction::HOLD && isHoldingKey(DIK_A)) {
		if (newAction == MarioAction::JUMP && canJump) {
			vy = -MARIO_JUMP_SPEED_Y;
			canJump = false;
		}

		return false;
	}

	// Refresh action duration
	if (state.action == newAction) state.beginAction = GetTickCount64();

	if (state.action == MarioAction::FALL_SLIGHTLY && newAction == MarioAction::IDLE) {
		state.action = MarioAction::IDLE;
		state.timeAction = 0;
	}


	switch (newAction)
	{
	case MarioAction::IDLE:
		/*if (action == MarioAction::IDLE || action == MarioAction::RUN || action == MarioAction::WALK
			|| action == MarioAction::FALL || action == MarioAction::CROUCH)*/
		SetAction(newAction, timeAction);
		break;

	case MarioAction::WALK:
		if (state.action == MarioAction::IDLE || state.action == MarioAction::RUN || state.action == MarioAction::CROUCH)
			SetAction(newAction, timeAction);
		break;

	case MarioAction::RUN:
		if (state.action == MarioAction::IDLE || state.action == MarioAction::WALK || state.action == MarioAction::SPEEDUP)
			SetAction(newAction, timeAction);
		break;

	case MarioAction::SPEEDUP:
		if (state.action == MarioAction::IDLE || state.action == MarioAction::RUN) SetAction(newAction, timeAction);
		break;

	case MarioAction::JUMP:
		if (state.action == MarioAction::IDLE || state.action == MarioAction::WALK || state.action == MarioAction::RUN
			|| state.action == MarioAction::CROUCH || state.action == MarioAction::SPEEDUP) {
			if (powerX < 6000) vy = -MARIO_JUMP_SPEED_Y;
			else {
				vy = -MARIO_JUMP_SPEED_Y * 1.21f;
				vx *= 1.2f;
			}
			SetAction(newAction, timeAction);
			if (state.action != MarioAction::CROUCH) SetAction(newAction, timeAction);

		}
		break;

	case MarioAction::HIGH_JUMP:
		if (state.action == MarioAction::JUMP && vy > -MARIO_JUMP_SPEED_Y * 0.6f && vy < -MARIO_JUMP_SPEED_Y * 0.55 && powerX < 6000) {

			vy = -MARIO_JUMP_SPEED_Y * 1.1f;
			SetAction(newAction, timeAction);
		}
		break;

	case MarioAction::CROUCH:
		if ((state.action == MarioAction::IDLE || state.action == MarioAction::WALK || state.action == MarioAction::RUN) && type != MarioType::SMALL)
			SetAction(newAction, timeAction);
		break;

	case MarioAction::FLY:
		if (powerX > 0) {
			if (state.action == MarioAction::FLY)
				vy = -MARIO_FLY_SPEED_Y;
			SetAction(newAction, timeAction);
		}
		break;

	case MarioAction::FALL:
		if (state.action == MarioAction::IDLE || state.action == MarioAction::RUN || state.action == MarioAction::WALK
			|| state.action == MarioAction::FLY || state.action == MarioAction::JUMP || state.action == MarioAction::FALL_SLIGHTLY
			|| state.action == MarioAction::HIGH_JUMP)
			SetAction(newAction, timeAction);
		break;

	case MarioAction::FALL_SLIGHTLY:
		if (state.action == MarioAction::FALL || state.action == MarioAction::FALL_SLIGHTLY) {
			//vy = 0;
			SetAction(newAction, timeAction);
		}

		break;

	case MarioAction::SKID:
		if ((state.action == MarioAction::RUN || state.action == MarioAction::WALK || state.action == MarioAction::SPEEDUP) && state.action != MarioAction::FLY && state.action != MarioAction::JUMP
			&& state.action != MarioAction::CROUCH) {
			vx = -nx * VELOCITY_X_AFTER_SKID;

			SetAction(newAction, timeAction);
		}

		break;
	case MarioAction::EXPLODE:
	case MarioAction::UPGRADE_LV:
		SetAction(newAction, 500);
		break;
	case MarioAction::ATTACK:
		if (state.action == MarioAction::IDLE || state.action == MarioAction::WALK
			|| state.action == MarioAction::RUN || state.action == MarioAction::JUMP || state.action == MarioAction::HIGH_JUMP) {
			SetAction(newAction, timeAction);
		}
		break;


	default:
		return false;
	}
	return true;
}

bool CMario::IsReadyToChangeAction() {
	if (GetTickCount64() < state.beginAction + state.timeAction) return false;
	return true;
}

#pragma endregion

#pragma region Respawn

void CMario::Reset()
{
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
}

void CMario::ResetTempValues() {
	ax = 0;
}

#pragma endregion

