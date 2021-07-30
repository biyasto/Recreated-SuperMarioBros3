#pragma once
#include "RectCollision.h"


class CTeleportGate : public CRectCollision
{
protected:
	Vector2 teleport_position;
	Vector2 LimitTopLeft;
	Vector2 LimitBotRight;
public:
	CTeleportGate(float, float, float, float,Vector2, Vector2, Vector2);
	void BeingCollided(LPGAMEOBJECT) override;
};

