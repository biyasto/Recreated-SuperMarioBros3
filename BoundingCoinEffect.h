#pragma once
#include "Effect.h"

class CBoundingCoinEffect : public CEffect
{
public:

	CBoundingCoinEffect(Vector2 initPos, Vector2 deltaPos) : CEffect(initPos, deltaPos) {};
	void OnFinish() override;
	std::string GetAnimationId() override;
	
};

