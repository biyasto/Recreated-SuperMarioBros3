#pragma once

#include <Windows.h>
#include <d3dx9.h>
#include <vector>

#include "SpriteManager.h"
#include "AnimationSet.h"
#include "PhysicsConsts.h"

class CCell;
typedef CCell* LPCell;

#define ID_TEX_BBOX "-100"		// special texture to draw object bounding box


class CGameObject;
typedef CGameObject* LPGAMEOBJECT;

struct CCollisionEvent;
typedef CCollisionEvent* LPCOLLISIONEVENT;
struct CCollisionEvent
{
	LPGAMEOBJECT obj;
	float t, nx, ny;

	float dx, dy;		// *RELATIVE* movement distance between this object and obj

	CCollisionEvent(float t, float nx, float ny, float dx = 0, float dy = 0, LPGAMEOBJECT obj = NULL)
	{
		this->t = t;
		this->nx = nx;
		this->ny = ny;
		this->dx = dx;
		this->dy = dy;
		this->obj = obj;
	}

	static bool compare(const LPCOLLISIONEVENT& a, LPCOLLISIONEVENT& b)
	{
		return a->t < b->t;
	}
};

struct SPrevBoundingBox {
	float left, top, right, bottom = 0;
};

struct SRenderAnimation {
	std::string AnimationID;
	bool isFlipY = false;
};

enum class EExtraEffect {
	NONE,
	BEING_DAMAGED,
	BONUS,
};

enum class EPriorityFlag {
	
	MAP_OBJECT, // QUESTIONBOX, GOLDEN BRICK,
	DYNAMIC_OBJECT, // Enemy
	DYNAMIC_OBJECT_BEHIND_MAP, // Venus
	MAIN_OBJECT, // Mario, Mario Bullets
	HIGH_PRIORITY_OBJECT,
	TEMP_OBJECT,
	ENEMY_BULLET
};

struct SExtraEffect {
	EExtraEffect type = EExtraEffect::NONE;
	int timeBegin = 0;
	int timeEffect = 0;
	Vector2 initPosition = Vector2(0, 0);
};




class CGameObject
{

public:

	SExtraEffect effect;
	bool allowOthersGoThrough = false;

	// Position
	float x;
	float y;

	// Velocity
	float vx;
	float vy;

	// Acceleration
	float ax;
	float ay;

	// Distance
	float dx;	// dx = vx*dt
	float dy;	// dy = vy*dt

	int powerX = 0;
	// Direction of Object ( Left , Right )
	int nx = 1;
	int ny = 1;

	LPCell cell;

	DWORD dt;

	std::vector<EPriorityFlag> priorityFlag;

	Vector2 basePosition = Vector2(0, 0);
	LPANIMATION_SET animation_set;

	bool VerifyCollidedLeftRight(LPGAMEOBJECT);

	bool isDisable = false;
	bool isTemp = false;
	SRenderAnimation renderAnimation;
	SPrevBoundingBox prevBoundingBox;

public:
	// Con/Destructor
	CGameObject();
	CGameObject(Vector2);
	~CGameObject();

	// Position 
	void GetPosition(float& x, float& y) { x = this->x; y = this->y; }
	Vector2 GetPosition() { return Vector2(this->x, this->y); }

	virtual Vector2 GetBasePosition() { return basePosition; }
	virtual void ChangeBasePosition(Vector2);
	void SetPosition(float x, float y) { this->x = x; this->y = y; }
	void SetPosition(Vector2 pos) { this->x = pos.x; this->y = pos.y; }
	// Speed
	void GetSpeed(float& vx, float& vy) { vx = this->vx; vy = this->vy; }
	void SetSpeed(float vx, float vy) { this->vx = vx, this->vy = vy; }
	
	bool AddPriority(EPriorityFlag);
	bool RemovePriority(EPriorityFlag);
	int GetNX() { return nx; }
	std::vector<EPriorityFlag> GetPriorityFlag() { return priorityFlag; }
	virtual std::string GetRenderAnimationId(EExtraEffect);

	virtual void SetEffect(EExtraEffect = EExtraEffect::NONE, DWORD = 0);
	virtual void SwitchEffect(EExtraEffect = EExtraEffect::NONE);
	virtual void RenderExtraEffect(Vector2);


	void ResetRenderAnimation();
	virtual void ChangeRenderAnimation(std::string newRenderAnimationID);
	virtual void ChangeRenderAnimation(SRenderAnimation newRenderAnimation);

	// BoundingBox
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) = 0;
	void RenderBoundingBox(Vector2 finalPos);

	void RenderMotionableAnimation(int);

	// AnimationSet
	void SetAnimationSet(LPANIMATION_SET ani_set) { animation_set = ani_set; }

	// Collision

	bool HasOverLap(Vector2, Vector2, Vector2, Vector2);

	LPCOLLISIONEVENT SweptAABBEx(LPGAMEOBJECT coO);
	void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents);
	void FilterCollision(
		vector<LPCOLLISIONEVENT>& coEvents,
		vector<LPCOLLISIONEVENT>& coEventsResult,
		float& min_tx,
		float& min_ty,
		float& nx,
		float& ny,
		float& rdx,
		float& rdy);

	// Physics
	void ApplyGravity();
	void ApplyNoGravity();
	void ApplyFriction();
	
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = nullptr);
	virtual void Render(Vector2 finalPos) {};

	virtual void OnHadCollided(LPGAMEOBJECT obj) { Collided(obj); };

	virtual void CollidedLeftRight(LPGAMEOBJECT);
	virtual void CollidedLeft(LPGAMEOBJECT);
	virtual void CollidedTop(LPGAMEOBJECT);
	virtual void CollidedRight(LPGAMEOBJECT);
	virtual void CollidedBottom(LPGAMEOBJECT);
	virtual void Collided(LPGAMEOBJECT) {};
	virtual void NoCollided() {};

	virtual void BeingCollided(LPGAMEOBJECT) {};
	virtual void BeingCollidedLeftRight(LPGAMEOBJECT obj) { BeingCollided(obj); }
	virtual void BeingCollidedLeft(LPGAMEOBJECT obj) { BeingCollidedLeftRight(obj); }
	virtual void BeingCollidedTopBottom(LPGAMEOBJECT obj) { BeingCollided(obj); }
	virtual void BeingCollidedTop(LPGAMEOBJECT obj) { BeingCollidedTopBottom(obj); }
	virtual void BeingCollidedRight(LPGAMEOBJECT obj) { BeingCollidedLeftRight(obj); }
	virtual void BeingCollidedBottom(LPGAMEOBJECT obj) { BeingCollidedTopBottom(obj); }

	void UpdateWithCollision(vector<LPGAMEOBJECT>* coObjects);
	void UpdateNoCollision();
	
	// Cell
	LPCell GetCell() { return this->cell; }
	void SetCell(LPCell cell) { this->cell = cell; }


};

