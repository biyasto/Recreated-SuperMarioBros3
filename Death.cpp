#include "Death.h"
#include "Mario.h"
#include "Enemy.h"

CDeath::CDeath(float x, float y, float width, float height) : CRectCollision(x, y, width, height) {

}

void CDeath::BeingCollided(LPGAMEOBJECT obj) {
	if (obj == NULL) return;
	if (dynamic_cast<CMario*>(obj)) {
		((CMario*)obj)->x -= 19;
		((CMario*)obj)->SetAction(MarioAction::DIE,500);

		if (dynamic_cast<CEnemy*>(obj)) {
			((CEnemy*)obj)->x -= 19;
			((CEnemy*)obj)->SetState(EEnemyState::DIE, 500);
			((CEnemy*)obj)->x = 0;
			((CEnemy*)obj)->y = 0;
			((CEnemy*)obj)->isDisable = true;

		}
	}
	else {
		obj->x = obj->y = 0;
		obj->isDisable = true;
	}
}

