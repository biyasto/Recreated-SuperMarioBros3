#include "CAdding1UpEffect.h"
#include "Game.h"

#define ADDING_1_UP	"ani-1Up"
#define DELTA_CHANGE_DELTA_POSITION_Y_ADDINGPOINT	0.0001f

std::string CAdding1UpEffect::GetAnimationId() {
	return ADDING_1_UP;
}

void CAdding1UpEffect::OnFinish() {
	CGame::GetInstance()->GetCurrentScene()->GetCamera()->AdjustScore(100);
}

void CAdding1UpEffect::Update(DWORD dt) {
	if (!isActive) return;
	currentPosition.y += deltaPosition.y * dt;
	deltaPosition.y += DELTA_CHANGE_DELTA_POSITION_Y_ADDINGPOINT * dt;
	if (deltaPosition.y > 0) {
		OnFinish();
		isActive = false;
	}
}