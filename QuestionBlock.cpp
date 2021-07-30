#include "QuestionBlock.h"
#include "PlayScene.h"
#include "BoundingCoinEffect.h"
#include "Enemy.h"
#include "TailAttackBox.h"



CQuestionBlock::CQuestionBlock(Vector2 initPos) :
	CRectCollision(initPos.x, initPos.y, QUESTION_BOX_SIZE, QUESTION_BOX_SIZE){
	state.type = EBlockState::DEFAULT;
	deltaRender = Vector2(0, 0);

}

void CQuestionBlock::BeingCollidedBottom(LPGAMEOBJECT) {
	ChangeState(EBlockState::OPENING);
}

void CQuestionBlock::OnHadCollided(LPGAMEOBJECT obj) {
	if (dynamic_cast<CTailAttackBox*>(obj)) {
		ChangeState(EBlockState::OPENING, 3000);

	}
}

void CQuestionBlock::Render(Vector2 finalPos) {
	CAnimations::GetInstance()->Get(GetAnimationIdFromState())->Render(Vector2(finalPos.x + deltaRender.x, finalPos.y+deltaRender.y));
	
}

void CQuestionBlock::BeingCollidedLeftRight(LPGAMEOBJECT obj) {
	if (dynamic_cast<CEnemy*>(obj)) {
		if (((CEnemy*)obj)->GetState() == EEnemyState::BEING_KICKED) {
			ChangeState(EBlockState::OPENING, 3000);
		}
	}
}


void CQuestionBlock::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	if (state.type == EBlockState::OPENING) {
		deltaRenderSpeed.y += 2.2f;
		deltaRender.y += deltaRenderSpeed.y;
		if (deltaRender.y > 0) {
			deltaRender.y = 0;
			deltaRenderSpeed.y = 0;
			ChangeState(EBlockState::OPENED);
		}
	}

}




std::string CQuestionBlock::GetAnimationIdFromState() {
	switch (state.type)
	{
	case EBlockState::DEFAULT:
		return QUESTION_BOX_ANIMATION_DEFAULT_ID;

	case EBlockState::OPENING:
	case EBlockState::OPENED:
		return QUESTION_BOX_ANIMATION_OPENED_ID;
	default:
		break;
	}
}




void CQuestionBlock::OpenBox() {
	deltaRenderSpeed.y = -15.5f;
	((CPlayScene*)(CGame::GetInstance()->GetCurrentScene()))->PushEffects(new CBoundingCoinEffect(Vector2(x,y-48*1.5), Vector2(0, -0.50)));
}




void CQuestionBlock::SetState(EBlockState newState, DWORD newTimeState) {
	state.type = newState;
	state.beginState = GetTickCount64();
	state.timeState = newTimeState;
}


bool CQuestionBlock::ChangeState(EBlockState newState, DWORD newTimeState) {
	if (GetTickCount64() < state.timeState + state.beginState) return false;
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