#pragma once
#include "GameObject.h"
#include "Mario.h"
#include "FireBullet.h"


enum class EEnemyState {
	ONESHOTDIE,
	DIE,
	WILL_DIE,
	BEING_KICKED,
	BEING_HELD,
	WILL_LIVE,
	LIVE,
	LIVE1,
	PREPARE_ATTACK,
	ATTACK
};

struct SEnemyState {
	EEnemyState type = EEnemyState::LIVE;
	int timeBegin = 0;
	int timeState = 0;
};



class CEnemy : public CGameObject
{
protected:
	SEnemyState state;
	CMario* holdController;
	Vector2 walkingScope;
	float walkingSpeed;

	bool useChangeDirectionAfterAxisCollide = false;

public:
	
	CEnemy();
	CEnemy(float, float, int = -1);

	void InitWtandingScope(LPGAMEOBJECT);

	void BeingHeldProcess();

	

	virtual Vector2 GetBoundingBoxSize() { return GetBoundingBoxSize(this->state.type); }
	virtual Vector2 GetBoundingBoxSize(EEnemyState) { return Vector2(0, 0); }
	virtual void GetBoundingBox(float&, float&, float&, float&);

	virtual void CollidedLeftRight(LPGAMEOBJECT) override;
	virtual void BeingCollidedTop(LPGAMEOBJECT) override;
	virtual void BeingCollided(LPGAMEOBJECT) override;
	virtual void BeingCollidedLeftRight(LPGAMEOBJECT) override;
	virtual void OnHadCollided(LPGAMEOBJECT) override;

	virtual void Render(Vector2);
	virtual void Update(DWORD, vector<LPGAMEOBJECT>*);
	void Update_Fly(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

	virtual float GetDefaultWalkingSpeed() { return 0.0f; }
	virtual void SetState(EEnemyState, DWORD = 0);
	virtual bool ChangeState(EEnemyState newState, DWORD = 0);
	virtual EEnemyState GetState() { return state.type; };
	virtual std::string GetAnimationIdFromState() = 0;

	virtual void ChangeDirection();
	virtual void ChangeDirectionAfterAxisCollide();


	virtual void BeingKicked(Vector2 pos);
	virtual void KillMario(CMario* );
};

