#pragma once
#include "QuestionBlock.h"
#define GOLDEN_BRICK_DEFAULT_ID					"ani-brick"
class GoldenBrickCoin : public CQuestionBlock
{
private:
	int numOfCoins = 5;
public:
	GoldenBrickCoin(Vector2);
	std::string GetAnimationIdFromState() override;
	bool ChangeState(EBlockState newState, DWORD newTimeState) override;
};

