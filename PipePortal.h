#pragma once
#include "GameObject.h"
#include "Transform.h"

class CPipePortal : public CGameObject
{
protected:

	Vector2 portalSize;
	Vector2 targetPosition;
	Vector2 targetSize;
	Vector2 targetCameraLeffTopLimit;
	Vector2 targetCameraRightBottomLimit;
	std::string accessKeycode;
public:
	CPipePortal(std::string, Vector2 = Vector2(0, 0), Vector2 = Vector2(0, 0), Vector2 = Vector2(0, 0), Vector2 = Vector2(0,0), Vector2 = Vector2(0, 0));
	void OnHadCollided(LPGAMEOBJECT) override;
	void GetBoundingBox(float&, float&, float&, float&) override;
	void Update(DWORD, vector<LPGAMEOBJECT>*);
	void Render(Vector2 finalPos) override;
	
};

