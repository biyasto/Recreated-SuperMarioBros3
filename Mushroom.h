#pragma once

#include "Leaf.h"

class Mushroom : public CLeaf
{
	bool PowerUp;
	const float vyMax = 0.186f;
public:
	Mushroom(Vector2);
	Mushroom(Vector2 initPos, bool isPowerUp);
	virtual void Collided(LPGAMEOBJECT) override;
	void Render(Vector2 finalPos) override;
	void Update(DWORD, vector<LPGAMEOBJECT>*) override;
	bool isPowerUp() { return PowerUp; };
};

