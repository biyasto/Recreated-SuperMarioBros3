#pragma once
#include "GameObject.h"
#include "Transform.h"
#include <string>

class CSelectionTree : public CGameObject
{
public:
	CSelectionTree(Vector2);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom) {};
	void Render(Vector2);

};

