#pragma once
#include "QuestionBlock.h"

enum class EHiddenItem {
	NONE,
	LEAF,
	P, 
	COIN,
	ONEUP
};


class CGoldenBrick : public CQuestionBlock
{
private:
	Vector2 virtualCenter;
	EHiddenItem hiddenItem = EHiddenItem::NONE;
public:
	CGoldenBrick(Vector2, std::string hiddenItem = "");

	bool HaveHiddenItem() { return (hiddenItem != EHiddenItem::NONE); };
	virtual void Render(Vector2 finalPos) override;
	Vector2 GetBoundingBoxSizeFromState();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom) override;
	void OnHadCollided(LPGAMEOBJECT) override;
	void BeingCollidedTop(LPGAMEOBJECT) override;
	void BeingCollidedBottom(LPGAMEOBJECT) override ;
	void BeingCollided(LPGAMEOBJECT) override;


	virtual void Update(DWORD, vector<LPGAMEOBJECT>*) override;
	virtual bool ChangeState(EBlockState, DWORD = 0);
	virtual void OpenBox();


	std::string GetAnimationIdFromState();


};