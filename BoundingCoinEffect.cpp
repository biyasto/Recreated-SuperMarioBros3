#include "BoundingCoinEffect.h"
#include "PlayScene.h"
#include "AddingPointEffect.h"

#define BOUNDING_COIN_ANIMATIONID	"ani-coin-in-question-block"


std::string CBoundingCoinEffect::GetAnimationId() {
	return BOUNDING_COIN_ANIMATIONID;
}

void CBoundingCoinEffect::OnFinish() {
	((CPlayScene*)(CGame::GetInstance()->GetCurrentScene()))->PushEffects(new CAdding100PointEffect(initialPosition, Vector2(0, -0.11)));
}