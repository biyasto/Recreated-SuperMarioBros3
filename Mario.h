#pragma once
#include "GameObject.h"
#include "MarioConsts.h"
#include "AnimationSet.h"
#include "Game.h"
#include <unordered_map>
using namespace std;

#define UNTOUCHABLE_TIME		3000

enum class MarioType
{
	SMALL,
	TALL,
	TAIL,
	FIRE
};


enum class MarioAction

{
	IDLE,
	WALK,
	RUN,
	SPEEDUP,
	JUMP,
	HIGH_JUMP,
	CROUCH,
	FLY,
	FALL,
	HOLD,
	FALL_SLIGHTLY,
	SKID,
	ATTACK,
	KICK,
	DIE,
	EXPLODE,
	UPGRADE_LV,
	GETTING_INTO_THE_PIPE
};

struct SGetInPipe {
	Vector2 distance = Vector2(0,50);
	DWORD time = 1500;
	const DWORD totalTime = 1500;
	CallbackTypeScene OnFinish = nullptr;
};

struct SMarioState {
	MarioAction action = MarioAction::IDLE;
	int beginAction = 0;
	int timeAction = 0; // thoi gian cho action
	
};

struct SUntouchable {
	bool isUntouchable = false;
	int remainingTime = 3000; //thoi gian con lai
};

class CMario : public CGameObject
{
protected:
	float start_x;			//ham reset dat vi tri ban dau
	float start_y;

	MarioType type;
	SMarioState state;
	
	Vector2 deltaRender = Vector2(0, 0);
	LPGAMEOBJECT holdingObj = NULL;
	SUntouchable untouchable;
	SGetInPipe getIntoThePipe;
	
	unordered_map<int, bool> holdingKeys;
	
	int finishStep = 0;
	bool canJump = false;

public:
	
	CMario(float x = 0.0f, float y = 0.0f);

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL) override;
	virtual void Render(Vector2 finalPos) override;

	virtual void ProcessKeyboard(SKeyboardEvent kEvent);
	bool isHoldingKey(int keyCode) { return holdingKeys[keyCode]; }

	std::string GetAnimationIdFromState();
	
	virtual void Collided(LPGAMEOBJECT) override;
	virtual void CollidedLeft(LPGAMEOBJECT) override;
	virtual void CollidedTop(LPGAMEOBJECT) override;
	virtual void CollidedBottom(LPGAMEOBJECT) override;
	virtual void BeingCollided(LPGAMEOBJECT) override;
	virtual void NoCollided() override;

	virtual Vector2 GetBoundingBoxSize();
	virtual Vector2 GetBoundingBoxSize(MarioType mType, MarioAction mAction);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	virtual void BeingKilled();
	virtual void BeingBouncedAfterJumpInTopEnemy();
	void BeginUntouchable();
	void GetIntoThePipe(Vector2, CallbackTypeScene = nullptr);
	void ChangeFinishStep(int a) { finishStep = a; }
	void BeingBouncedAfterJumpInNoteBlock();
	void SecretBeingBouncedAfterJumpInNoteBlock();
	void Teleport(float tele_x = 0.0f, float tele_y = 0.0f, float vel_x = 0.0f, float vel_y = 0.0f);
	void SetHoldingObj(LPGAMEOBJECT obj) { holdingObj = obj;}
	virtual void SetAction(MarioAction newAction, DWORD timeAction = 0);
	virtual MarioAction GetAction() { return state.action; }
	virtual bool ChangeAction(MarioAction newAction, DWORD timeAction = 0);
	bool IsReadyToChangeAction();

	void SetType(MarioType a) { type = a; }
	virtual MarioType GetType() { return type; }
	int GetFinishStep() { return finishStep; }
	void Reset();
	void ResetTempValues();

};
