#pragma once
#include "RectCollision.h"


class CDeath : public CRectCollision
{
public:
	CDeath(float, float, float, float);
	void BeingCollided(LPGAMEOBJECT) override;
};

