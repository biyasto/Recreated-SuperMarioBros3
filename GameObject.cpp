#include <d3dx9.h>


#include <algorithm>

#include "debug.h"
#include "TextureManager.h"
#include "Game.h"
#include "GameObject.h"
#include "RectPlatform.h"
#include "PhysicConstants.h"

#include "QuestionBlock.h"
#include "Mario.h"



#define ANIMATIONID_BEING_DAMAGE		"ani-enemy-damaged"
#define ANIMATIONID_BONUS				"ani-mario-damaged"
#define DELTA_TO_APPLY_NEW_BASE_POSITION		500

CGameObject::CGameObject()
{
	x = y = 0;
	vx = vy = 0;
	nx = 1;
	priorityFlag = std::vector<EPriorityFlag>();
}

CGameObject::CGameObject(Vector2 pos)
{
	x = pos.x;
	y = pos.y;
	vx = vy = 0;
	nx = 1;
	priorityFlag = std::vector<EPriorityFlag>();
}

bool CGameObject::AddPriority(EPriorityFlag flag) {
	for (int i = 0; i < priorityFlag.size(); i++) {
		if (priorityFlag[i] == flag) return false;
	}
	priorityFlag.push_back(flag);
	return true;
}

bool CGameObject::RemovePriority(EPriorityFlag flag) {
	for (int i = 0; i < priorityFlag.size(); i++) {
		if (priorityFlag[i] == flag) {
			priorityFlag.erase(priorityFlag.begin() + i);
			return true;
		}
	}
	
	return false;
}

void CGameObject::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	this->dt = dt;
	dx = vx * dt;
	if (vy > GRAVITY_VELOCITY_MAX) vy = GRAVITY_VELOCITY_MAX;
	dy = vy * dt;
}


bool CGameObject::HasOverLap(Vector2 l1 , Vector2 r1, Vector2 l2, Vector2 r2) {
	if (l1.x >= r2.x || l2.x >= r1.x)
		return false;

	// If one rectangle is above other 
	if (l1.y >= r2.y || l2.y >= r1.y)
 		return false;

	return true;
}


/*
	Extension of original SweptAABB to deal with two moving objects
*/
LPCOLLISIONEVENT CGameObject::SweptAABBEx(LPGAMEOBJECT coO)
{
	float sl, st, sr, sb;		// static object bbox
	float ml, mt, mr, mb;		// moving object bbox
	float t, nx, ny;

	coO->GetBoundingBox(sl, st, sr, sb);

	// deal with moving object: m speed = original m speed - collide object speed
	float svx, svy;
	coO->GetSpeed(svx, svy);

	float sdx = svx * dt;
	float sdy = svy * dt;

	// (rdx, rdy) is RELATIVE movement distance/velocity 
	float rdx = this->dx - sdx;
	float rdy = this->dy - sdy;

	GetBoundingBox(ml, mt, mr, mb);

	CGame::SweptAABB(
		ml, mt, mr, mb,
		rdx, rdy,
		sl, st, sr, sb,
		t, nx, ny
		);

	

	if (ml < mr && mt < mb && sl != sr && st != sb) {
		if (
			HasOverLap(Vector2(ml, mt), Vector2(mr, mb), Vector2(sl, st), Vector2(sr, sb))
			) {

			coO->OnHadCollided(this);
			this->OnHadCollided(coO);
		}
	}
	
	

	CCollisionEvent* e = new CCollisionEvent(t, nx, ny, rdx, rdy, coO);
	return e;
}

/*
	Calculate potential collisions with the list of colliable objects

	coObjects: the list of colliable objects
	coEvents: list of potential collisions
*/
void CGameObject::CalcPotentialCollisions(
	vector<LPGAMEOBJECT>* coObjects,
	vector<LPCOLLISIONEVENT>& coEvents)
{
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		//// Bounding Box of GameObject
		//float oLeft, oTop, oRight, oBottom;
		//this->GetBoundingBox(oLeft, oTop, oRight, oBottom);

		//// Bounding Box cua TargetObject
		//float tLeft, tTop, tRight, tBottom;
		//coObjects->at(i)->GetBoundingBox(tLeft, tTop, tRight, tBottom);

		//if (
		//	(oLeft >= tLeft && oLeft <= tRight) &&
		//	(oTop >= tTop && oTop <= oBottom) &&
		//	(oRight > oLeft && oBottom > oTop) // ensure that this object has collision box != 0,0,0,0
		//)
		//{
		//}
		//else {
		//	
		//}

		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));

		if (e->t > 0 && e->t <= 1.0f)
			coEvents.push_back(e);
		else
			delete e;

		
	}

	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);
}

void CGameObject::FilterCollision(
	vector<LPCOLLISIONEVENT>& coEvents,
	vector<LPCOLLISIONEVENT>& coEventsResult,
	float& min_tx, float& min_ty,
	float& nx, float& ny, float& rdx, float& rdy)
{
	min_tx = 1.0f;
	min_ty = 1.0f;
	int min_ix = -1;
	int min_iy = -1;

	nx = 0.0f;
	ny = 0.0f;

	coEventsResult.clear();

	for (UINT i = 0; i < coEvents.size(); i++)
	{
		LPCOLLISIONEVENT c = coEvents[i];

		if (c->t < min_tx && c->nx != 0) {
			
			if (dynamic_cast<CRectPlatform*>(c->obj)) {
				// Do not thing ( allow Mario go in X-direction )
			}
			else {
				min_tx = c->t; nx = c->nx; min_ix = i; rdx = c->dx;
			}
			
		}

		if (c->t < min_ty && c->ny != 0) {
			if (dynamic_cast<CRectPlatform*>(c->obj) && c->ny == 1) {
				min_ty = 1; ny = 2; min_iy = i; rdy = c->dy;
			}
			else {
				min_ty = c->t; ny = c->ny; min_iy = i; rdy = c->dy;
			}
			
		}
	}

	if (min_ix >= 0) coEventsResult.push_back(coEvents[min_ix]);
	if (min_iy >= 0) coEventsResult.push_back(coEvents[min_iy]);
}

void CGameObject::RenderMotionableAnimation(int totalStep) {

}

void CGameObject::RenderBoundingBox(Vector2 finalPos)
{
	
	RECT rect;

	LPDIRECT3DTEXTURE9 bbox = CTextures::GetInstance()->Get(ID_TEX_BBOX);

	float l, t, r, b;

	GetBoundingBox(l, t, r, b);
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l;
	rect.bottom = (int)b - (int)t;

	
	//CGame::GetInstance()->Draw(finalPos, Vector2(0,0), bbox, rect, 222);
	CGame::GetInstance()-> DrawWithScaling(finalPos, Vector2(0,0), bbox, rect, 222);

}

void CGameObject::ApplyGravity() {
	
	if (vy >= GRAVITY_VELOCITY_MAX) return;
	if (vy + ACCELERATION_Y_GRAVITY * dt < GRAVITY_VELOCITY_MAX)
		vy += ACCELERATION_Y_GRAVITY * dt;
	else
		vy = GRAVITY_VELOCITY_MAX;
}

void CGameObject::ApplyNoGravity() {


	
}

void CGameObject::ApplyFriction() {
	if (vx > 0) {
		vx += -ACCELERATION_FRICTION*dt;
		if (vx < 0) vx = 0;
	}
	if (vx < 0) {
		vx += ACCELERATION_FRICTION*dt;
		if (vx > 0) vx = 0;
	}
}

void CGameObject::ChangeBasePosition(Vector2 newPos) {
	if (abs(basePosition.x - newPos.x) > DELTA_TO_APPLY_NEW_BASE_POSITION) basePosition.x = newPos.x;
	if (abs(basePosition.y - newPos.y) > DELTA_TO_APPLY_NEW_BASE_POSITION) basePosition.y = newPos.y;
	
}

void CGameObject::CollidedLeftRight(LPGAMEOBJECT obj) {
	Collided(obj);
	obj->BeingCollidedLeftRight(this);
}
void CGameObject::CollidedLeft(LPGAMEOBJECT obj) { CollidedLeftRight(obj); };
void CGameObject::CollidedRight(LPGAMEOBJECT obj) { CollidedLeftRight(obj); };
void CGameObject::CollidedTop(LPGAMEOBJECT obj) {
	Collided(obj);
	obj->BeingCollidedTop(this);
};
void CGameObject::CollidedBottom(LPGAMEOBJECT obj) {
	Collided(obj);
	obj->BeingCollidedBottom(this);
};

void CGameObject::UpdateNoCollision() {
	x += dx;
	y += dy;
}


bool CGameObject::VerifyCollidedLeftRight(LPGAMEOBJECT obj) {
	
	float ml, mt, mr, mb, sl, st, sr, sb = 0;
	obj->GetBoundingBox(sl, st, sr, sb);
	this->GetBoundingBox(ml, mt, mr, mb);
	if ((mb>st && sb>mt) || abs(vy) > 0.1f) return true;
	return false;
}

void CGameObject::UpdateWithCollision(vector<LPGAMEOBJECT>* coObjects) {
	
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	
	
	coEvents.clear();
	
	CalcPotentialCollisions(coObjects, coEvents);
	
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
		NoCollided();

	}
	else
	{
		
		if (coEvents.size() == 2) {
			int a = 9;
		}
		
		float min_tx, min_ty, nx = 0, ny = 0;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		

		for (int i = 0; i < coEventsResult.size(); i++) {
			
			if (coEventsResult[i]->nx != 0) {
				if (!VerifyCollidedLeftRight(coEventsResult[i]->obj) || coEventsResult[i]->obj->allowOthersGoThrough) {
				//if (coEventsResult[i]->obj->allowOthersGoThrough) {
					min_tx = 1;
				}
				else {
					vx = 0;
				}
				if (coEventsResult[i]->nx > 0) CollidedRight(coEventsResult[i]->obj);
				else CollidedLeft(coEventsResult[i]->obj);
			}
			else if (coEventsResult[i]->ny != 0) {
				if (coEventsResult[i]->obj->allowOthersGoThrough) {
					min_ty = 1;
				}
				else {
					if (ny != 2) vy = 0;
				}
				if (coEventsResult[i]->ny > 0) CollidedBottom(coEventsResult[i]->obj);
				else CollidedTop(coEventsResult[i]->obj);
			}
			
		}

		
		// block every object first!
		x += min_tx * dx + nx * 0.4f;
		//if(ny != 0)
		y += min_ty * dy + ny * 0.4f;
		

		
		

	}

	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

}


void CGameObject::RenderExtraEffect(Vector2 finalPos) {
	if (effect.type != EExtraEffect::NONE)
		CAnimations::GetInstance()->Get(CGameObject::GetRenderAnimationId(effect.type))->Render(
			Vector2(finalPos.x + (effect.initPosition.x - this->x), finalPos.y + (effect.initPosition.y - this->y)),
			Vector2(-nx, ny), 255);
}

void CGameObject::ResetRenderAnimation() {
	renderAnimation.AnimationID = "";
	renderAnimation.isFlipY = false;
}

void CGameObject::ChangeRenderAnimation(std::string newRenderAnimationID) {
	// Completed rendering last animation 
	ResetRenderAnimation();
	renderAnimation.AnimationID = newRenderAnimationID;
}

void CGameObject::ChangeRenderAnimation(SRenderAnimation newRenderAnimation) {
	// Completed rendering last animation 
	renderAnimation = newRenderAnimation;
}

CGameObject::~CGameObject()
{

}



void CGameObject::SwitchEffect(EExtraEffect eff) {
	SetEffect(eff, 500);
	effect.initPosition = Vector2(this->x, this->y);
}

void CGameObject::SetEffect(EExtraEffect newEffect, DWORD timeEffect) {
	if (GetTickCount64() < static_cast<unsigned long long>(effect.timeEffect) + effect.timeBegin) return;
	effect.timeBegin = GetTickCount64();
	effect.timeEffect = timeEffect;
	effect.type = newEffect;
}



std::string CGameObject::GetRenderAnimationId(EExtraEffect effctType) {
	switch (effctType)
	{
	case EExtraEffect::BEING_DAMAGED:
		return ANIMATIONID_BEING_DAMAGE;
	case EExtraEffect::BONUS:
		return ANIMATIONID_BONUS;
	default:

		return ANIMATIONID_BEING_DAMAGE;
	}
}
