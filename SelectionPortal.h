#pragma once
#include "GameObject.h"
#include "Transform.h"
#include <string>

class CSelectionPortal : public CGameObject
{
	
	Vector2 size;
	std::string spriteId;

public:
	CSelectionPortal(Vector2, Vector2, std::string);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom) {};
	void Render(Vector2);

};

