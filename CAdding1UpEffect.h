#pragma once
#include "Effect.h"
class CAdding1UpEffect : public CEffect
{
public:
	CAdding1UpEffect(Vector2 initPos, Vector2 deltaPos) : CEffect(initPos, deltaPos) {};
	virtual std::string GetAnimationId() override;
	void Update(DWORD) override;
	void OnFinish() override;
};
