#include "KoopasFly.h"

CKoopasFly::CKoopasFly(float x, float y) : CRedGoomba(x, y)
{

}


std::string CKoopasFly::GetAnimationIdFromState() {
	switch (state.type)
	{
	case EEnemyState::LIVE: {
		return "ani-green-koopa-paratroopa-fly";
	}

	case EEnemyState::LIVE1:
		return "ani-green-koopa-troopa-move";


	case EEnemyState::ONESHOTDIE:
		return "ani-green-koopa-troopa-move";
	case EEnemyState::WILL_DIE:
		return "ani-green-koopa-troopa-crouch";
	default:
		return "ani-green-koopa-troopa-move";
	}
}

void CKoopasFly::ProcessHasWingBehavior() {
	vy = -0.55f;
}

Vector2 CKoopasFly::GetBoundingBoxSize(EEnemyState st) {
	switch (st)
	{
	case EEnemyState::LIVE:
	case EEnemyState::LIVE1:
		return Vector2(48, 77);


	case EEnemyState::WILL_DIE:
		return Vector2(48, 40);
	case EEnemyState::DIE:
	case EEnemyState::ONESHOTDIE:
		return Vector2(0, 0);
	default:
		return Vector2(GOOMBA_BBOX_WIDTH, GOOMBA_BBOX_HEIGHT);
	}
}