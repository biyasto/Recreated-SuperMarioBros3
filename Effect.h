#pragma once
#include "Transform.h"
#include <string>
#define	DEFAULT_ANIMATIONID		"ani-coin"
#define	DEFAULT_CHANGE_DELTA_POSITION_X			0.0f
#define	DEFAULT_CHANGE_DELTA_POSITION_Y			0.0011f

class CEffect
{
protected:
	Vector2 initialPosition;
	Vector2 currentPosition;
	Vector2 deltaPosition;
	bool isActive = true;

public:
	CEffect(Vector2, Vector2);
	Vector2 GetCurrentPosition() { return currentPosition; }
	bool GetActiveState() { return isActive; }
	virtual void OnFinish() {};
	virtual std::string GetAnimationId() { return DEFAULT_ANIMATIONID; }
	virtual void Update(DWORD);
	void Render(Vector2);
};

