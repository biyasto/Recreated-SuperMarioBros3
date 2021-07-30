#include "Effect.h"
#include "Animation.h"



CEffect::CEffect(Vector2 initPos, Vector2 deltaPos) {
	initialPosition = initPos;
	currentPosition = initPos;
	deltaPosition = deltaPos;

}

void CEffect::Update(DWORD dt) {
	if (!isActive) return;
	currentPosition.y += deltaPosition.y * dt;
	deltaPosition.y += DEFAULT_CHANGE_DELTA_POSITION_Y * dt;
	if (currentPosition.y > initialPosition.y) {
		OnFinish();
		isActive = false;
	}
}

void CEffect::Render(Vector2 finalPos) {
	CAnimations::GetInstance()->Get(GetAnimationId())->Render(finalPos);
}