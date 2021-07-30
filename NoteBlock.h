#pragma once
#include "QuestionBlock.h"
using namespace std;
class CNoteBlock : public CQuestionBlock
{
private:
	Vector2 virtualCenter;
	int BoundCount;
	bool Secret;
public:
	CNoteBlock(Vector2,string hiddenItem = "");


	virtual void Render(Vector2 finalPos) override;
	Vector2 GetBoundingBoxSizeFromState();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom) override;
	void OnHadCollided(LPGAMEOBJECT) override;
	void BeingCollidedTop(LPGAMEOBJECT) override;
	void BeingCollidedBottom(LPGAMEOBJECT) override;
	void BeingCollided(LPGAMEOBJECT) override;


	virtual void Update(DWORD, vector<LPGAMEOBJECT>*) override;



	std::string GetAnimationIdFromState();


};

