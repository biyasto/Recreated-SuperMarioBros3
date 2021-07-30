#include "FireMario.h"


CFireMario::CFireMario(float x, float y) : CMario(x, y) {
	type = MarioType::FIRE;
	bullets.push_back(new CFireBullet(0.0f, 0.0f, 1));
	bullets.push_back(new CFireBullet(0.0f, 0.0f, 1));
}


bool CFireMario::ShootBullet() {
	for (int i = 0; i < bullets.size(); i++) {
	
		if (bullets.at(i)->isDisable) {
			bullets.at(i)->PrepareForShooting();
			bullets.at(i)->UpdatePos(Vector2(this->x, this->y), this->nx);
			bullets.at(i)->isDisable = false;
			return true;
		}
	}
	return false;
}






void CFireMario::ProcessKeyboard(SKeyboardEvent kEvent)
{
	CMario::ProcessKeyboard(kEvent);

	switch (kEvent.key)
	{
	case DIK_A:
		if (!kEvent.isHold && !kEvent.isKeyUp)
			if (ShootBullet())
				ChangeAction(MarioAction::ATTACK, 200);
		break;

	}
}