#pragma once
#include "GameObject.h"

struct SClaimBonusEffect {
	bool isActive = false;
	Vector2 totalMoveDistance = Vector2(0, -350);
	int remainingTime;
	const int totalTime = 1000;
};

class CEndSceneItem : public CGameObject
{
	Vector2 deltaRender = Vector2(0, 0);
	const Vector2 size = Vector2(48, 48);
	SClaimBonusEffect claimBonusEffect;

	int cardId = 0;
	int cardIdRemainingTime = 0;

public:
	void Collided(LPGAMEOBJECT) override;
	CEndSceneItem(Vector2);
	void Update(DWORD, vector<LPGAMEOBJECT>*) override;
	void Render(Vector2 finalPos) override;
	void ChangeCardId(DWORD);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom) override;
};

