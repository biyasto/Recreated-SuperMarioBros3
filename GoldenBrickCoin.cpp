#include "GoldenBrickCoin.h"
#include "PlayScene.h"

GoldenBrickCoin::GoldenBrickCoin(Vector2 initPos) : CQuestionBlock(initPos) {

}

std::string GoldenBrickCoin::GetAnimationIdFromState() {
	switch (state.type)
	{
	case EBlockState::DEFAULT:
		return GOLDEN_BRICK_DEFAULT_ID;

	case EBlockState::OPENING:
	case EBlockState::OPENED:
		return QUESTION_BOX_ANIMATION_OPENED_ID;
	default:
		break;
	}
}


bool GoldenBrickCoin::ChangeState(EBlockState newState, DWORD newTimeState) {
	if (GetTickCount64() < static_cast<unsigned long long>(state.timeState) + state.beginState) return false;
	switch (newState)
	{
	case EBlockState::DEFAULT:
		SetState(newState, newTimeState);
		break;
	case EBlockState::OPENING:
		if (state.type == EBlockState::DEFAULT) {
			if (dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())) {
				((CPlayScene*)(CGame::GetInstance()->GetCurrentScene()))->PushTempObjects(this);
				isTemp = true;
			}
			OpenBox();
			numOfCoins--;
			if(numOfCoins < 1)
				SetState(newState, newTimeState);
		}

		break;

	case EBlockState::OPENED:
		SetState(newState, newTimeState);
		isTemp = false;
		break;

	default:
		break;
	}
	return true;
}
