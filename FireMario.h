#pragma once
#include "Mario.h"
#include "FireBullet.h"

class CFireMario : public CMario
{
private:
	vector<CFireBullet*> bullets;

public:
	CFireMario(float x = 0.0f, float y = 0.0f);
	void ProcessKeyboard(SKeyboardEvent kEvent);
	CFireBullet* GetBullet(int index) { return bullets.at(index); }
	bool ShootBullet();
};

