#pragma once
#include "GameObject.h"

class CSelectionSceneMario : public CGameObject
{
public:
	CSelectionSceneMario(Vector2);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom) {};
	void Render(Vector2);
};

