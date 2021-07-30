#pragma once
#include "GameObject.h"
#include "Mario.h"


class CLeaf : public CGameObject
{
	const float vyMax = 0.056f;
	int beginFalling = 0;
	Vector2 initPosition;
	MarioType type; //1=nam,2=la,3+=hoa
public:
	CLeaf(Vector2);
	virtual void Render(Vector2) override;
	virtual void Update(DWORD, vector<LPGAMEOBJECT>*) override;	
	virtual void Collided(LPGAMEOBJECT) override;
	virtual void GetBoundingBox(float&, float&, float&, float&) override;
	void BehaviorLeaf(DWORD, vector<LPGAMEOBJECT>*);
	void BehaviorFlower(DWORD, vector<LPGAMEOBJECT>*);
};

