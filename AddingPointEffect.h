#pragma once
#include "Effect.h"
class CAdding100PointEffect : public CEffect
{
public:
	CAdding100PointEffect(Vector2 initPos, Vector2 deltaPos) : CEffect(initPos, deltaPos) {};
	virtual std::string GetAnimationId() override;
	void Update(DWORD) override;
	void OnFinish() override;
};

