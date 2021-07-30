#pragma once
#include "RectCollision.h"
#define QUESTION_BOX_SIZE				48
#define QUESTION_BOX_ANIMATION_DEFAULT_ID		"ani-question-block"
#define QUESTION_BOX_ANIMATION_OPENED_ID		"ani-empty-block"
#define QUESTION_BOX_OPEN_TOTAL_TIME			200
#define QUESTION_BOX_OPEN_EACH_TIME				20

enum class EBlockState {
	DEFAULT,
	OPENING,
	OPENED,
	SHOW_HIDDEN_COIN
};



struct SBlockState {
	EBlockState type = EBlockState::DEFAULT;
	DWORD beginState = 0;
	DWORD timeState = 0;
};

class CQuestionBlock : public CRectCollision
{
protected:
	SBlockState state;
	Vector2 deltaRender;
	Vector2 deltaRenderSpeed;


public:
	
	

	CQuestionBlock(Vector2 initPos);
	virtual void Render(Vector2 finalPos);


	virtual void Update(DWORD, vector<LPGAMEOBJECT>* ) override;
	virtual bool ChangeState(EBlockState, DWORD = 0);
	virtual void SetState(EBlockState, DWORD = 0);
	virtual void OpenBox();

	virtual void BeingCollidedLeftRight(LPGAMEOBJECT) override;
	
	virtual std::string GetAnimationIdFromState();
	
	void OnHadCollided(LPGAMEOBJECT) override;
	void BeingCollidedBottom(LPGAMEOBJECT) override;
};



