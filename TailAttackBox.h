#pragma once
#include "GameObject.h"


class CTailAttackBox : public CGameObject
{
private:
	Vector2 size = Vector2(80, 10);
	bool hasAttacked = true;
	bool isOpening = true;
public:
	CTailAttackBox();
	virtual void GetBoundingBox(float&, float&, float&, float&) override;
	void Update(DWORD, vector<LPGAMEOBJECT>* = NULL) override;
	void UpdatePosition(Vector2);
	void SetHasAttacked(bool a) { hasAttacked = a; }
	void SetIsOpening(bool a);
	void Render(Vector2) override;

	void OnHadCollided(LPGAMEOBJECT) override;
	
};

