#pragma once
#include "GameObject.h"
#define COIN_SIZE		48

class CCoin : public CGameObject
{

public:
	CCoin(Vector2);
	void Render(Vector2) override;
	void Update(DWORD, vector<LPGAMEOBJECT>*) override;
	void GetBoundingBox(float&, float&, float&, float&) override;

	void BeingCollided(LPGAMEOBJECT) override;
	

};

