#include "SelectionPortal.h"
#include "SpriteManager.h"

CSelectionPortal::CSelectionPortal(Vector2 pos, Vector2 siz, std::string spr) {
	this->x = pos.x;
	this->y = pos.y;
	this->size = siz;
	this->spriteId = spr;
}

void CSelectionPortal::Render(Vector2 finalPos)
{
	std::string sprId = "spr-" + this->spriteId;
	CSprites::GetInstance()->Get(sprId)->DrawWithScaling(finalPos);
}
