#include "IntroSceneObj.h"
#include "SpriteManager.h"
#include "Animation.h"

CIntroSceneObj::CIntroSceneObj(Vector2 pos, std::string name) {
	this->x = pos.x;
	this->y = pos.y;
	this->name = name;
}

SIntroRender CIntroSceneObj::GetIntroRender(std::string name) {
	SIntroRender result;
	if (name == "Curtain") {
		result.isSprite = true;
		result.id = "spr-top-curtain-0";
	}
	else if (name == "Title") {
		result.isSprite = true;
		result.id = "spr-title-0";
	}
	else if (name == "StartOption1") {
		result.isSprite = true;
		result.id = "spr-option-0";
	}
	else if (name == "StartOption2") {
		result.isSprite = true;
		result.id = "spr-option-1";
	}
	else if (name == "CopyRight") {
		result.isSprite = true;
		result.id = "spr-option-2";
	}
	else if (name == "OptionCursor") {
		result.isSprite = true;
		result.id = "spr-option-arrow-0";
	}
	else if (name == "Ani3") {
		result.isSprite = false;
		result.id = "ani-3";
	}

	return result;
}

void CIntroSceneObj::Render(Vector2 finalPos)
{
	SIntroRender a = GetIntroRender(this->name);
	if (a.isSprite) {
		CSprites::GetInstance()->Get(a.id)->DrawWithScaling(finalPos + deltaRender);
	}
	else {
		CAnimations::GetInstance()->Get(a.id)->Render(finalPos + deltaRender);
	}
	
}
