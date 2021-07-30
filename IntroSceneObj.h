#pragma once
#include "GameObject.h"


struct SIntroRender {
	
	bool isSprite;
	std::string id;
};

class CIntroSceneObj : public CGameObject
{
	Vector2 deltaRender = Vector2(0,0);
	std::string name;

public:
	CIntroSceneObj(Vector2, std::string);
	std::string GetName() { return name; }
	void ChangeDeltaRender(Vector2 a) { deltaRender = a; }
	void GetBoundingBox(float& left, float& top, float& right, float& bottom) {};
	void Render(Vector2);
	SIntroRender GetIntroRender(std::string);

};

