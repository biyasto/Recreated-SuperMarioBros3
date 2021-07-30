#pragma once
#include "GameObject.h"

class CRectPlatform : public CGameObject
{
private:
	int width, height;
public:
	CRectPlatform(float x, float y, float width, float height);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Render(Vector2 finalPos);

};

